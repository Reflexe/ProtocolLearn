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

#include "SocketInterface.h"

#include <net/if_arp.h>  // ARPHRD_ETHER

namespace ProtocolLearn {

SocketInterface::SocketInterface(const Interface &interface)
    : mInterface{interface} {
    sockaddr_ll sockaddr;

    ::memset(&sockaddr, 0, sizeof(sockaddr));

//    macAddress.copyNumeric(sockaddr.sll_addr);     /* Address */
    sockaddr.sll_ifindex = static_cast<int>(interface.getIndex());   /* Interface Index */
    sockaddr.sll_halen = 0;
    sockaddr.sll_hatype = ARPHRD_ETHER;
    sockaddr.sll_family = AF_PACKET;
    sockaddr.sll_pkttype = PACKET_HOST;
    sockaddr.sll_protocol = 0;

    SocketAddress socketAddress(sockaddr);
    mSocket.bind(socketAddress);
}

OctetVector SocketInterface::_recv(const Timeout &timeout) {
    {
        auto timeToWait = timeout.howMuchTimeDoWeHave();

        if (timeToWait != mCurrentTimeout)
            mSocket.setTimeout(timeToWait);
    }

    OctetVector data;

    try{
        mSocket.receive(data, mInterface.getMTU(), 0);

    } catch(Socket::SocketException &e) {
        if(e.getErrorCode() == EWOULDBLOCK)
            throw Timeout::TimeoutException{"SocketInterface::_recv"};
    }

    return data;
}

void SocketInterface::_send(OctetVector &&data) {
    try
    {
        mSocket.send(data);
    } catch(Socket::SocketException &) {
        pl_assert(false);
    }
}

void SocketInterface::setPromiscuousMode(bool status) {
    packet_mreq packet_req;

    ::memset(&packet_req, 0, sizeof(packet_req));

    packet_req.mr_ifindex = static_cast<int>(mInterface.getIndex());
    packet_req.mr_type = PACKET_MR_PROMISC;
    packet_req.mr_alen = MacAddress::MacAddressLength;

    int optionName = 0;
    if(status)
        optionName = PACKET_ADD_MEMBERSHIP;
    else
        optionName = PACKET_DROP_MEMBERSHIP;

    mSocket.setSocketOption(SOL_PACKET, optionName, static_cast<void *>(&packet_req), sizeof(packet_req));
}

MacAddress SocketInterface::getMacAddress()
{
    return mInterface.getMacAddress();
}

Ipv4Address SocketInterface::getIpv4Address()
{
    return mInterface.getIpv4Address();
}

Ipv4Address SocketInterface::getIpv4NetworkMask()
{
    return mInterface.getIpv4NetworkMask();
}

Ipv6Address SocketInterface::getIpv6Address()
{
    return mInterface.getIpv6Address();
}

Ipv6Address SocketInterface::getIpv6NetworkMask()
{
    return mInterface.getIpv6NetworkMask();
}

OctetVector::SizeType SocketInterface::getMaximumSendDataLength()
{
    return mInterface.getMTU();
}

struct SocketInterfaceFork : SocketInterface::BasicInterfaceFork{
    SocketInterfaceFork(const Interface &interface);

    virtual BasicInterface &getBasicInterface();

    SocketInterface socketInterface;
};

std::unique_ptr<BasicInterface::BasicInterfaceFork> SocketInterface::fork()
{
    return std::unique_ptr<BasicInterfaceFork>{new SocketInterfaceFork{mInterface}};
}

SocketInterfaceFork::SocketInterfaceFork(const Interface &interface)
    : socketInterface{interface}
{

}

BasicInterface &SocketInterfaceFork::getBasicInterface()
{
    return socketInterface;
}

} // namespace ProtocolLearn

