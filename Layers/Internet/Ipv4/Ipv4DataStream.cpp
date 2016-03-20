/**
 * @file ipv4datastream.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
  Ipv4DataStream
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) 2015 shrek0
 *
 * ProtocolLearn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProtocolLearn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 */

#include "Ipv4DataStream.h"

#include <netinet/ip.h>
#include <functional>
#include <limits>

#include "Random.h"
#include "IcmpDataStream.h"
#include "Ipv4Fragmenter.h"

namespace ProtocolLearn {
namespace Ipv4 {

Ipv4DataStream::Ipv4DataStream(Ipv4Stream &ipv4Stream, const Ipv4Address &destination,
                               uint8_t protocol, const Ipv4Address &source,
                               bool enableFragmention)
    : DataStreamUnderPacketStream{ipv4Stream},
      mIsFragmentionEnabled{enableFragmention},
      mpFragmentReassemblers{enableFragmention ? new Ipv4FragmentReassemblyManager{} : nullptr} {

    getSendPacket().setDestination(destination);
    getSendPacket().setSource(source);
    getSendPacket().setProtocol(protocol);

    getPacketStream().getFilter().filterByPacket(getSendPacket());
}

void Ipv4DataStream::_send(OctetVector &&data) {
    // Update the ID before the sending.
    getSendPacket().setIdentification(static_cast<uint16_t>(Random::getMediumRandomNumber()));
    getSendPacket().importData(std::move(data));

    if(isFragmentionEnabled() == false) {
        pl_assert(getMaximumSendDataLength() >= data.size());

        getSendPacket().setDontFragment(true);
        getPacketStream().sendPacket(getSendPacket());
    } else {
        sendWithFragmention(getSendPacket());
    }

    getSendPacket().removeData();
}

OctetVector Ipv4DataStream::_recv() {
    if(mIsFragmentionEnabled) {
        if (receiveWithFragmention(getReceivePacket()) == false)
            return OctetVector{};
        else
            return std::move(getReceivePacket().getVectorData());
    } else {
        return DataStreamUnderPacketStream::_recv();
    }
}

OctetVector::SizeType Ipv4DataStream::getMaximumSendDataLength() {
    pl_assert(getPacketStream().getDataStream().getMaximumSendDataLength() > PacketType::MinimumHeaderLength);

    if(isFragmentionEnabled()) // The fragment offset is uint16_t.
        return (std::numeric_limits<uint16_t>::max() * 8)
                + (getPacketStream().getDataStream().getMaximumSendDataLength() - PacketType::MinimumHeaderLength);
    else
        return getPacketStream().getDataStream().getMaximumSendDataLength()-PacketType::MinimumHeaderLength;
}

OctetVector::SizeType Ipv4DataStream::getRealMaximumSendDataLength()
{
    return getMaximumSendDataLength();
}

bool Ipv4DataStream::isFragmentionEnabled() const
{
    return mIsFragmentionEnabled;
}

void Ipv4DataStream::setFragmentionStatus(bool status) {
    if(mIsFragmentionEnabled != status) {
        if(status == true)
            mpFragmentReassemblers.reset(new Ipv4FragmentReassemblyManager{});
        else
            mpFragmentReassemblers.reset();
    }

    mIsFragmentionEnabled = status;
}

void Ipv4DataStream::trySaveMemory() {
    pl_assert(isFragmentionEnabled());
    mpFragmentReassemblers->tryClearOldEntries();
}

struct Ipv4DataStreamFork : Ipv4DataStream::InternetProtocolFork{
    Ipv4DataStreamFork(std::unique_ptr<LinkProtocol::LinkProtocolFork> &&linkProtocol,
                       const Ipv4Address &destination,
                       uint8_t protocol,
                       const Ipv4Address &source, bool enableFragmention);

    virtual InternetProtocol &getInternetProtocol() override;

    std::unique_ptr<LinkProtocol::LinkProtocolFork> linkProtocolFork;
    Ipv4Stream ipv4Stream;
    Ipv4DataStream ipv4DataStream;
};


InternetProtocol &Ipv4DataStreamFork::getInternetProtocol()
{
    return ipv4DataStream;
}

Ipv4DataStreamFork::Ipv4DataStreamFork(std::unique_ptr<LinkProtocol::LinkProtocolFork> &&linkProtocol,
                                       const Ipv4Address &destination,
                                       uint8_t protocol,
                                       const Ipv4Address &source,
                                       bool enableFragmention)
    : linkProtocolFork{std::move(linkProtocol)},
      ipv4Stream{linkProtocolFork->getLinkProtocol()},
      ipv4DataStream{ipv4Stream, destination,
                     protocol,
                     source,
                     enableFragmention}
{
}

std::unique_ptr<InternetProtocol::InternetProtocolFork> Ipv4DataStream::fork(bool reply) {
    auto &sendPacket = getSendPacket();

    return std::unique_ptr<InternetProtocolFork>{new Ipv4DataStreamFork{getPacketStream().getDataStream().fork(reply),
                    reply ? getReceivePacket().getSource() : sendPacket.getDestination(),
                    sendPacket.getProtocol(),
                    sendPacket.getSource(),
                    isFragmentionEnabled()}};
}

bool Ipv4DataStream::receiveWithFragmention(Ipv4Packet &packet) {
    getPacketStream().receivePacket(packet);

    if(Ipv4FragmentReassembler::isFragment(packet)) {
        if(mpFragmentReassemblers->enterPacket(std::move(packet)) == false)
            return false;
    }

    return true;
}

void Ipv4DataStream::sendWithFragmention(Ipv4Packet &packet) {
    if(packet.getPacketLength() > getPacketStream().getDataStream().getMaximumSendDataLength()) {
        auto fragments = Ipv4Fragmenter::fragmentPacket(packet, getPacketStream().getDataStream().getMaximumSendDataLength());

        for(auto &fragment : fragments)
        {
            getPacketStream().sendPacket(fragment);
        }
    } else {
        getPacketStream().sendPacket(packet);
    }
}

} // Ipv4
} // ProtocolLearn
