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

#ifndef TCPIPV4PACKET_H
#define TCPIPV4PACKET_H

#include "TcpPacket.h"

namespace ProtocolLearn {
namespace Tcpv4 {

struct Tcpv4PseudoHeader{
    uint32_t sourceAddress;
    uint32_t destinationAddress;
    uint8_t reserved = 0;
    uint8_t protocol = 6; // IPPROTO_TCP.
    uint16_t tcpPacketLength = 0;
}__attribute__((packed));

typedef Tcp::TcpPacket TcpIpv4Packet;

}
}

#endif // TCPIPV4PACKET_H
