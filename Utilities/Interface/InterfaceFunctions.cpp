/**
 * @file netfunctions.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
  InterfaceFunctions
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
 * Contain a class that provide helper functions for Interface class.
 */

#include "InterfaceFunctions.h"

#include <net/if_arp.h> // For ARPHRD_ETHER

#include "ByteOrder.h"

namespace ProtocolLearn {

void InterfaceFunctions::initIfr(ifreq &ifr)
{
    ::memset(&ifr, 0, sizeof(ifr));
}

void InterfaceFunctions::setName(const std::string &name, ifreq &ifr) const{
    if(name.size() > IFNAMSIZ)
        throw InvalidArgument("NAME length is larger than IFNAMSIZ");

    strncpy(ifr.ifr_ifrn.ifrn_name, name.c_str(), name.size());
}

void InterfaceFunctions::ioctlTalk(ulong type, ifreq &ifr) const{
    if(::ioctl(socket.getFileDescriptor(), type, &ifr) < 0)
        throw Socket::SocketException("Cannot use ioctl function");
}

Ipv4Address InterfaceFunctions::getIpv4AddressByInterfaceName(const std::string &name) const{
    sockaddr_in *pSockaddr_in = nullptr;
    ifreq ifr;

    initIfr(ifr);
    setName(name,ifr);

    /* It's Ipv4 address */
    ifr.ifr_ifru.ifru_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_ifrn.ifrn_name, name.c_str(), name.size());

    ioctlTalk(SIOCGIFADDR, ifr);

    pSockaddr_in = reinterpret_cast<sockaddr_in *>(&ifr.ifr_ifru.ifru_addr);

    return Ipv4Address(ByteOrder::networkToHost(pSockaddr_in->sin_addr.s_addr));
}

MacAddress InterfaceFunctions::getMacAddressByInterfaceName(const std::string &name) const{
    ifreq ifr;

    initIfr(ifr);

    setName(name, ifr);

    ifr.ifr_ifru.ifru_addr.sa_family = ARPHRD_ETHER;

    ioctlTalk(SIOCGIFHWADDR, ifr);

    return MacAddress::fromNumeric(reinterpret_cast<uint8_t *>(ifr.ifr_hwaddr.sa_data));
}

Ipv4Address InterfaceFunctions::getIpv4DestinationAddress(const std::string &name) const{
    sockaddr_in *pSockaddr_in = nullptr;
    ifreq ifr;

    initIfr(ifr);
    setName(name, ifr);

    /* It's Ipv4 address */
    ifr.ifr_ifru.ifru_addr.sa_family = AF_INET;

    ioctlTalk(SIOCGIFDSTADDR, ifr);

    pSockaddr_in = reinterpret_cast<sockaddr_in *>(&ifr.ifr_ifru.ifru_dstaddr);

    return Ipv4Address(ByteOrder::networkToHost(pSockaddr_in->sin_addr.s_addr));
}

Ipv6Address InterfaceFunctions::getIpv6Address(const std::string &name) const{
    sockaddr_in6 *pSockaddr_in = nullptr;
    ifreq ifr;

    initIfr(ifr);
    setName(name,ifr);

    /* It's Ipv6 address */
    ifr.ifr_ifru.ifru_addr.sa_family = AF_INET6;

    strncpy(ifr.ifr_ifrn.ifrn_name, name.c_str(), name.size());

    ioctlTalk(SIOCGIFADDR, ifr);

    pSockaddr_in = reinterpret_cast<sockaddr_in6 *>(&ifr.ifr_ifru.ifru_addr);

    ByteOrder::networkToHost(pSockaddr_in->sin6_addr.__in6_u.__u6_addr8, 16);

    return Ipv6Address{pSockaddr_in->sin6_addr.__in6_u.__u6_addr8};
}

Ipv6Address InterfaceFunctions::getIpv6Mask(const std::string &name) const{
    sockaddr_in6 *pSockaddr_in = nullptr;
    ifreq ifr;

    initIfr(ifr);
    setName(name,ifr);

    /* It's Ipv6 address */
    ifr.ifr_ifru.ifru_addr.sa_family = AF_INET6;

    strncpy(ifr.ifr_ifrn.ifrn_name, name.c_str(), name.size());

    ioctlTalk(SIOCGIFNETMASK, ifr);

    pSockaddr_in = reinterpret_cast<sockaddr_in6 *>(&ifr.ifr_ifru.ifru_addr);

    ByteOrder::networkToHost(pSockaddr_in->sin6_addr.__in6_u.__u6_addr8, 16);

    return Ipv6Address{pSockaddr_in->sin6_addr.__in6_u.__u6_addr8};
}

Ipv4Address InterfaceFunctions::getIpv4MaskAddress(const std::string &name) const{
    sockaddr_in *pSockaddr_in = nullptr;
    ifreq ifr;

    initIfr(ifr);
    setName(name, ifr);

    /* It's Ipv4 address */
    ifr.ifr_ifru.ifru_addr.sa_family = AF_INET;

    ioctlTalk(SIOCGIFNETMASK, ifr);

    pSockaddr_in = reinterpret_cast<sockaddr_in *>(&ifr.ifr_ifru.ifru_netmask);

    return Ipv4Address(ByteOrder::networkToHost(pSockaddr_in->sin_addr.s_addr));
}

uint32_t InterfaceFunctions::getMTUByInterfaceName(const std::string &name) const{
    ifreq ifr;
    initIfr(ifr);

    setName(name, ifr);
    ifr.ifr_ifru.ifru_mtu = 0;

    ioctlTalk(SIOCGIFMTU, ifr);

    return ifr.ifr_ifru.ifru_mtu;
}

} // ProtocolLearn
