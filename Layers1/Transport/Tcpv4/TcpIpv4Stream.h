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

#ifndef TCPIPV4STREAM_H
#define TCPIPV4STREAM_H

#include "DataStreamUnderPacketStream.h"

#include "TcpStream.h"
#include "TcpIpv4Packet.h"

#include "Ipv4DataStream.h"
#include "IPProtocolIpv4.h"

namespace ProtocolLearn {
namespace Tcpv4 {

class TcpIpv4Stream : public Tcp::TcpStream
{
public:
    TcpIpv4Stream(Ipv4::Ipv4DataStream &dataStream);

//    /// Update the pseudo header before the sending and after the receiving.
//    virtual void _send(Tcp::TcpPacket &packet) override final;
//    virtual void _recv(Tcp::TcpPacket &packet) override final;

    const Ipv4::Ipv4DataStream &getIpv4DataStream() const{ return mIpv4DataStream; }
    Ipv4::Ipv4DataStream &getIpv4DataStream() { return mIpv4DataStream; }

private:
//    void updatePseudoHeaderByIpv4Packet(Tcpv4PseudoHeader &tcpPseudoHeader, const Ipv4::Ipv4Packet &ipv4Packet, uint16_t tcpPacketLength);

    Ipv4::IPProtocolIpv4 mIPProtocol;
    Ipv4::Ipv4DataStream &mIpv4DataStream;

};

} // Tcpv4
} // ProtocolLearn

#endif // TCPIPV4STREAM_H
