/**
 * @file interface.h
 * @author shrek0 (shrek0.tk@gmail.com)
  Interface
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
 * Provides a class for Interfaces managing.
 */

#include "Interface.h"

#include "InterfaceFunctions.h"

namespace ProtocolLearn {

Interface::Interface()
    : d{"any", 0}
{
}

Interface::Interface(const std::string &name)
    : d{name, inerfaceNameToInterfaceIndex(name)}
{
}

Interface::Interface(Interface::InterfaceIndex index) {
    char *pName = NULL;
    char buffer[IF_NAMESIZE];

    pName = if_indextoname(index, buffer);

    if(pName == NULL)
        throw InvalidArgument("Cannot convert Index to an Interface Name");

    d.name = pName;
    d.index = index;
}

Interface Interface::fromName(const std::string &name)
{
    return Interface(name);
}

Interface Interface::fromName(const char *name)
{
    return Interface(std::string(name));
}

Interface Interface::fromIndex(Interface::InterfaceIndex index)
{
    return Interface(index);
}

Interface::InterfaceIndex Interface::getIndex() const
{
    return d.index;
}

const std::string &Interface::getName() const
{
    return d.name;
}

const Ipv4Address Interface::getIpv4Address() const
{
    return InterfaceFunctions{}.getIpv4AddressByInterfaceName(d.name);
}

const MacAddress Interface::getMacAddress() const
{
    return InterfaceFunctions{}.getMacAddressByInterfaceName(d.name);
}

const Ipv4Address Interface::getIpv4NetworkDestination() const
{
    return InterfaceFunctions{}.getIpv4DestinationAddress(d.name);
}

const Ipv4Address Interface::getIpv4NetworkMask() const
{
    return InterfaceFunctions{}.getIpv4MaskAddress(d.name);
}

Ipv6Address Interface::getIpv6Address() const
{
    return InterfaceFunctions{}.getIpv6Address(d.name);
}

Ipv6Address Interface::getIpv6NetworkMask() const
{
    return InterfaceFunctions{}.getIpv6Mask(d.name);
}

uint32_t Interface::getMTU() const
{
    return InterfaceFunctions{}.getMTUByInterfaceName(d.name);
}

Interface::InterfaceIndex Interface::inerfaceNameToInterfaceIndex(const std::string &name) {
    if(name == "any")
        return 0;

    InterfaceIndex interfaceIndex = if_nametoindex(name.c_str());

    if(interfaceIndex == 0)
        throw InvalidArgument("name");

    return interfaceIndex;
}

}
