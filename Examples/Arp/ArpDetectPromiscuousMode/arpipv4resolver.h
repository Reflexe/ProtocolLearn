/**
 * @file arpipv4resolver.h
 * @author shrek0 (shrek0.tk@gmail.com)
 * @class ArpIpv4Resolver
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

#ifndef ARPIPV4RESOLVER_H
#define ARPIPV4RESOLVER_H

#include <Interface>

#include <EthernetStream>
#include <EthernetDataStream>

#include <ArpStream>
#include <ArpDataStream>

#include <ArpEthernetIpv4Stream>

using namespace ProtocolLearn;

class ArpIpv4Resolver
{
public:
    ArpIpv4Resolver(const Ipv4Address &ipv4Address);
    ArpIpv4Resolver(const Interface &interface);

    MacAddress resolve(const Ipv4Address &ipv4Address);

    virtual ~ArpIpv4Resolver();

private:
    void init(const Ipv4Address &ipv4Address);
    void init(const Interface &interface);

    void setEthernetFilter(ProtocolFilter<EthernetPacket> &ethernetFilter);

    struct Data{
        LowSocket socket;
        Interface interface;
    };

    Data d;

    struct Classes{
        EthernetStream *ethernetStream;
        EthernetDataStream *ethernetDataStream;

        ArpStream *arpStream;
        ArpDataStream *arpDataStream;

        ArpEthernetIpv4Stream *arpEthernetIpv4Stream;
    };

    Classes classes;
};

#endif // ARPIPV4RESOLVER_H
