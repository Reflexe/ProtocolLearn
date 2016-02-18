/**
 * @file interfacefunctions.h
 * @author shrek0 (shrek0.tk@gmail.com)
  NetFunctions
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

#ifndef INTERFACEFUNCTIONS_H
#define INTERFACEFUNCTIONS_H

#include <cstring>

#include <sys/ioctl.h>
#include <net/if.h>

#include "Ipv4Address.h"
#include "Ipv6Address.h"
#include "MacAddress.h"

#include "Socket.h"

namespace ProtocolLearn {

class InterfaceFunctions
{
public:
    Socket socket = Socket(AF_INET, SOCK_DGRAM);

    static void initIfr(ifreq &ifr);

    void ioctlTalk(ulong type, ifreq &ifr) const;

    void setName(const std::string &name, ifreq &ifr) const;

public:
    Ipv4Address getIpv4AddressByInterfaceName(const std::string &name) const;

    MacAddress getMacAddressByInterfaceName(const std::string &name) const;

    uint32_t getMTUByInterfaceName(const std::string &name) const;
    Ipv4Address getIpv4MaskAddress(const std::string &name) const;
    Ipv4Address getIpv4DestinationAddress(const std::string &name) const;

    Ipv6Address getIpv6Address(const std::string &name) const;
    Ipv6Address getIpv6Mask(const std::string &name) const;
};

} // ProtocolLearn

#endif // INTERFACEFUNCTIONS_H
