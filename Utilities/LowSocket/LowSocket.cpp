/**
 * @file lowsocket.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
  LowSocket
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
 * Provides a class for Low-Level sockets input/output.
 */

#include "LowSocket.h"

namespace ProtocolLearn {

LowSocket::LowSocket(int type, int protocol, int domain)
    : SocketDataStream(domain, type, protocol)
{
}

LowSocket::LowSocket(const Interface &interface, const MacAddress &macAddress, int type, int protocol, int domain) :
    LowSocket(type, protocol, domain)
{
    configEthernetSocket(interface, macAddress);
}

LowSocket::LowSocket(const LowSocket::Config &config)
    : LowSocket(config.interface, config.macAddress)
{
}

void LowSocket::configEthernetSocket(const Interface &interface, const MacAddress &macAddress) {
    sockaddr_ll sockaddr;

    mInterface = interface;
    mMacAddress = macAddress;

    ::memset(&sockaddr, 0, sizeof(sockaddr));

    macAddress.copyNumeric(sockaddr.sll_addr);     /* Address */
    sockaddr.sll_ifindex = interface.getIndex();   /* Interface Index */
    sockaddr.sll_halen = MacAddress::MacAddressLength;
    sockaddr.sll_hatype = ARPHRD_ETHER;
    sockaddr.sll_family = AF_PACKET;
    sockaddr.sll_pkttype = PACKET_HOST;
    sockaddr.sll_protocol = 0;

    SocketAddress socketAddress(sockaddr);

    getSocket().bind(socketAddress);
}

void LowSocket::setPromiscuousMode(bool status) {
    packet_mreq packet_req;

    packet_req.mr_ifindex = mInterface.getIndex();
    packet_req.mr_type = PACKET_MR_PROMISC;
    packet_req.mr_alen = MacAddress::MacAddressLength;
    mMacAddress.copyNumeric(packet_req.mr_address);

    int optionName = 0;
    if(status)
        optionName = PACKET_ADD_MEMBERSHIP;
    else
        optionName = PACKET_DROP_MEMBERSHIP;

    getSocket().setSocketOption(SOL_PACKET, optionName, reinterpret_cast<void *>(&packet_req), sizeof(packet_req));
}

} // ProtocolLearn
