/**
 * @file ethernetdatastream.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
  EthernetDataStream
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

#include "EthernetDataStream.h"

namespace ProtocolLearn {
namespace Ethernet {

struct EthernetFork : EthernetDataStream::LinkProtocolFork{
    EthernetFork(std::unique_ptr<BasicInterface::BasicInterfaceFork> &&basicInterface,
                 const MacAddress &destination, uint16_t protocol, const MacAddress &source);

    virtual LinkProtocol &getLinkProtocol() override;

    EthernetStream ethernetStream;
    EthernetDataStream ethernetDataStream;
};

EthernetFork::EthernetFork(std::unique_ptr<BasicInterface::BasicInterfaceFork> &&basicInterface, const MacAddress &destination, uint16_t protocol,
                           const MacAddress &source)
    : LinkProtocolFork{std::move(basicInterface)},
      ethernetStream{mBasicInterfaceFork->getBasicInterface()},
      ethernetDataStream{ethernetStream, destination, protocol, source}
{
}

LinkProtocol &EthernetFork::getLinkProtocol()
{
    return ethernetDataStream;
}

EthernetDataStream::EthernetDataStream(EthernetStream &ethernetStream, const MacAddress &destination, uint16_t protocol, const MacAddress &source)
    : DataStreamUnderPacketStream{ethernetStream} {
    mSendPacket.setDestination(destination);
    mSendPacket.setSource(source);
    mSendPacket.setProtocol(protocol);

    // Get reply for our packet only.
    getPacketStream().getFilter().filterByPacket(mSendPacket);
}

BasicInterface &EthernetDataStream::getBasicInterface()
{
    return getPacketStream().getDataStream();
}

std::unique_ptr<LinkProtocol::LinkProtocolFork> EthernetDataStream::fork(bool reply) {
    auto &sendPacket = getSendPacket();

    return std::unique_ptr<LinkProtocolFork>{new EthernetFork{getPacketStream().getDataStream().fork(),
                    reply ? getReceivePacket().getSource() : sendPacket.getDestination(),
                    sendPacket.getProtocol(),
                    sendPacket.getSource()}};

}

OctetVector::SizeType EthernetDataStream::getMaximumSendDataLength()
{
    return getBasicInterface().getMaximumSendDataLength()-EthernetPacket::MinimumHeaderLength;
}

} // Ethernet
} // ProtocolLearn
