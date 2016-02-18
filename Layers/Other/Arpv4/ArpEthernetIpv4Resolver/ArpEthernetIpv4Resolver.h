/**
 * @file arpethernetipv4resolver.h
 * @author shrek0 (shrek0.tk@gmail.com)
  ArpEthernetIpv4Resolver
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

#ifndef ARPETHERNETIPV4RESOLVER_H
#define ARPETHERNETIPV4RESOLVER_H

#include "Interface.h"

#include "SocketInterface.h"

#include "EthernetStream.h"
#include "EthernetDataStream.h"

#include "ArpStream.h"
#include "ArpDataStream.h"

#include "ArpEthernetIpv4Stream.h"

namespace ProtocolLearn {

namespace Libraries {

/**
 * @brief The ArpEthernetIpv4Resolver class  Resolve a MacAddress by an Ipv4Address with no headache.
 */
class ArpEthernetIpv4Resolver
{
public:
    /**
     * @brief ArpEthernetIpv4Resolver  A constructor for only one Ipv4Address.
     * @param ipv4Address  Ipv4 Address in the LAN.
     * @throw InvalidArgument  If @arg ipv4Address isn't a local Ipv4Address.
     */
    explicit ArpEthernetIpv4Resolver(const Ipv4Address &ipv4Address);
    explicit ArpEthernetIpv4Resolver(const Interface &interface);
    ~ArpEthernetIpv4Resolver();

    MacAddress resolve(const Ipv4Address &ipv4Address);

private:
    static Interface getInterfaceByIpv4Address(const Ipv4Address &ipv4Address);

    SocketInterface mSocketInterface;

    Ethernet::EthernetStream mEthernetStream;
    Ethernet::EthernetDataStream mEthernetDataStream;

    Arp::ArpStream mArpStream;
    Arp::ArpDataStream mArpDataStream;

    Arpv4::ArpEthernetIpv4Stream mArpEthernetIpv4Stream;
};

} // Libraries

} // ProtcolLearn

#endif // ARPETHERNETIPV4RESOLVER_H
