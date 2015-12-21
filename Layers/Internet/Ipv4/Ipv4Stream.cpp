#include "Ipv4Stream.h"

#include "Ipv4Fragmenter.h"

namespace ProtocolLearn {
namespace Ipv4 {

Ipv4Stream::Ipv4Stream(LinkProtocol &linkProtocol)
    : PacketStreamUnderDataStream{linkProtocol}
{
}

void Ipv4Stream::receiveWithFragmention(Ipv4Packet &packet) {
    receivePacket(packet);

    if(Ipv4FragmentReassembler::isFirstFragment(packet))
        receiveFragmentedPacket(packet, packet);
}

void Ipv4Stream::sendWithFragmention(Ipv4Packet &packet) {
    if(packet.getPacketLength() > getDataStream().getMaximumSendDataLength() && Ipv4FragmentReassembler::isFirstFragment(packet) == false
            && Ipv4FragmentReassembler::isFragment(packet) == false) {
        auto fragments = Ipv4Fragmenter::fragmentPacket(packet, getDataStream().getMaximumSendDataLength());

        for(auto &fragment : fragments)
        {
            sendPacket(fragment);
        }
    } else {
        sendPacket(packet);
    }
}

void Ipv4Stream::receiveFragmentedPacket(const Ipv4Packet &firstPacket, Ipv4Packet &packetToReceive) {
    pl_debug("Receiving a fragmented packet");

    // second/30 == 30Hz.
    Timeout fragmentTimeout{0, 33333, true};
    Ipv4FragmentReassembler ipv4FragmentReassembler{firstPacket};
    Ipv4Packet receivedPacket;
    auto savedTimeout = getTimeout();

    setTimeout(PTime{0, 33333});

    do {
        if(fragmentTimeout.check() == true)
            throw Timeout::TimeoutException("Ipv4Stream::receiveFragmentedPacket"); // Send an ICMP.

        receivePacket(receivedPacket);

        if(ipv4FragmentReassembler.isPacketRequired(receivedPacket))
            ipv4FragmentReassembler.insertPacket(receivedPacket);
    } while(ipv4FragmentReassembler.isAllOffsetsFilled() == false);

    setTimeout(savedTimeout);

    packetToReceive = ipv4FragmentReassembler.toPacket();
}

} // ProtocolLearn
} // Ipv4
