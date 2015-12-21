/**
 * @file
 * @author shrek0 (shrek0.tk@gmail.com)
 
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

#include "TcpIpv4Stream.h"

#include "ByteOrder.h"

namespace ProtocolLearn {
namespace Tcpv4 {

TcpIpv4Stream::TcpIpv4Stream(Ipv4::Ipv4DataStream &dataStream)
    : TcpStream{mIPProtocol}, mIPProtocol{dataStream}, mIpv4DataStream{dataStream} // May be buggy: mIPProtocol hasn't initilized yet.
{
}

//void TcpIpv4Stream::_send(Tcp::TcpPacket &packet) {
//    // Update the Pseudo header before the sending.

//    packet.getPseudoHeader().resize(sizeof(Tcpv4PseudoHeader));
//    updatePseudoHeaderByIpv4Packet(packet.getPseudoHeader().getAsObject<Tcpv4PseudoHeader>(),
//                                   mIpv4DataStream.getSendPacket(), packet.getPacketLength());

//    TcpStream::_send(packet);
//}

//void TcpIpv4Stream::_recv(Tcp::TcpPacket &packet) {
//    TcpStream::_recv(packet);

//    packet.getPseudoHeader().resize(sizeof(Tcpv4PseudoHeader));
//    updatePseudoHeaderByIpv4Packet(packet.getPseudoHeader().getAsObject<Tcpv4PseudoHeader>(),
//                                   mIpv4DataStream.getReceivePacket(), packet.getPacketLength());
//}

//void TcpIpv4Stream::updatePseudoHeaderByIpv4Packet(Tcpv4PseudoHeader &tcpPseudoHeader, const Ipv4::Ipv4Packet &ipv4Packet, uint16_t tcpPacketLength) {
//    tcpPseudoHeader.protocol = 6; // IPPROTO_TCP.
//    tcpPseudoHeader.reserved = 0;
//    tcpPseudoHeader.destinationAddress = ByteOrder::hostToNetwork(ipv4Packet.getDestination().toInt());
//    tcpPseudoHeader.sourceAddress = ByteOrder::hostToNetwork(ipv4Packet.getSource().toInt());
//    tcpPseudoHeader.tcpPacketLength = ByteOrder::hostToNetwork(tcpPacketLength);
//}

} // Tcpv4
} // ProtocolLearn
