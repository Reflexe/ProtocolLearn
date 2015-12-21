/**
 * @file %{FILENAME}
 * @author shrek0 (shrek0.tk@gmail.com)
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) %YEAR% shrek0
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

#include "IPProtocolIpv4.h"

#include <limits>

#include "Debug.h"

namespace ProtocolLearn {
namespace Ipv4 {

IPProtocolIpv4::IPProtocolIpv4(Ipv4DataStream &ipv4DataStream)
    : IPProtocolStream{ipv4DataStream}, mIpv4DataStream{ipv4DataStream}
{
}

void IPProtocolIpv4::updatePseudoHeaderRecv(IPProtocolPacket &packet)
{
    updatePseudoHeaderByPacket(packet.getPseudoHeader(), mIpv4DataStream.getReceivePacket(), packet.getPacketLength());
}

void IPProtocolIpv4::updatePseudoHeaderSend(IPProtocolPacket &packet)
{
    updatePseudoHeaderByPacket(packet.getPseudoHeader(), mIpv4DataStream.getSendPacket(), packet.getPacketLength());
}

struct IPProtocolIpv4Fork : IPProtocolStream::IPProtocolStreamFork {
    IPProtocolIpv4Fork(IPProtocolIpv4 &ipprotocolIpv4, bool reply);
    virtual IPProtocolStream &getIPProtocol() override;

    IPProtocolIpv4 ipProtocolIpv4;
};

IPProtocolIpv4Fork::IPProtocolIpv4Fork(IPProtocolIpv4 &ipprotocolIpv4, bool reply)
    : IPProtocolStreamFork{ipprotocolIpv4.getDataStream().fork(reply)},
      ipProtocolIpv4{static_cast<Ipv4DataStream&>(mInternetProtocol->getInternetProtocol())}
{
}

std::unique_ptr<IPProtocolStream::IPProtocolStreamFork> IPProtocolIpv4::fork(bool reply)
{
    return std::unique_ptr<IPProtocolStreamFork>{new IPProtocolIpv4Fork{*this, reply}};
}

void IPProtocolIpv4::updatePseudoHeaderByPacket(OctetVector &pseudoHeader, const Ipv4Packet &ipv4Packet,
                                                OctetVector::SizeType segementLength) {
    Ipv4PseudoHeader ipv4PseudoHeader;

    pl_assert(segementLength <= std::numeric_limits<uint16_t>::max());

    ipv4PseudoHeader.destinationAddress = ByteOrder::hostToNetwork(ipv4Packet.getDestination().toInt());
    ipv4PseudoHeader.sourceAddress = ByteOrder::hostToNetwork(ipv4Packet.getSource().toInt());
    ipv4PseudoHeader.segementLength = ByteOrder::hostToNetwork(static_cast<uint16_t>(segementLength));
    ipv4PseudoHeader.protocol = ipv4Packet.getProtocol();
    ipv4PseudoHeader.reserved = 0;

    pseudoHeader = OctetVector::fromObject(ipv4PseudoHeader);
}

IPProtocolStream &IPProtocolIpv4Fork::getIPProtocol()
{
    return ipProtocolIpv4;
}

} // namespace Ipv4
} // namespace ProtocolLearn

