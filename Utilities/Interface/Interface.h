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

#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>

#include "Ipv4Address.h"
#include "MacAddress.h"
#include "Ipv6Address.h"

namespace ProtocolLearn {

class Interface
{
public:
    typedef uint InterfaceIndex;

    Interface();

    explicit Interface(const std::string &name);

    explicit Interface(InterfaceIndex index);

    static Interface fromName(const std::string &name);

    static Interface fromName(const char *name);

    static Interface fromIndex(InterfaceIndex index);

    InterfaceIndex getIndex() const;

    const std::string &getName() const;

    const Ipv4Address getIpv4Address() const;

    const MacAddress getMacAddress() const;

    const Ipv4Address getIpv4NetworkDestination() const;

    const Ipv4Address getIpv4NetworkMask() const;

    Ipv6Address getIpv6Address() const;
    Ipv6Address getIpv6NetworkMask() const;

    uint32_t getMTU() const;

private:
    InterfaceIndex inerfaceNameToInterfaceIndex(const std::string &name);

    struct Data{
        std::string name;
        InterfaceIndex index;
    };

    Data d;
};

} // ProtocolLearn

#endif // INTERFACE_H
