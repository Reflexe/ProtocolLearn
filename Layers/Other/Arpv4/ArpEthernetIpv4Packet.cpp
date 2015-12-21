#include "ArpEthernetIpv4Packet.h"

namespace ProtocolLearn {
namespace Arpv4 {

ArpEthernetIpv4Packet::ArpEthernetIpv4Packet()
{
}

void ArpEthernetIpv4Packet::onPacketImport() {
    arpMiniPacket.senderIpv4Address = ByteOrder::networkToHost(getHeader().senderIpv4Address);
    arpMiniPacket.targetIpv4Address = ByteOrder::networkToHost(getHeader().targetIpv4Address);

    arpMiniPacket.senderMacAddress = getHeader().senderMacAddress;
    arpMiniPacket.targetMacAddress = getHeader().targetMacAddress;
}

void ArpEthernetIpv4Packet::onPacketExport() {
    /* Update the packet header */
    getHeader().senderIpv4Address = ByteOrder::hostToNetwork(arpMiniPacket.senderIpv4Address.toInt());
    getHeader().targetIpv4Address = ByteOrder::hostToNetwork(arpMiniPacket.targetIpv4Address.toInt());

    arpMiniPacket.senderMacAddress.copyNumeric(getHeader().senderMacAddress);
    arpMiniPacket.targetMacAddress.copyNumeric(getHeader().targetMacAddress);
}

} // Arpv4
} // ProtocolLearn
