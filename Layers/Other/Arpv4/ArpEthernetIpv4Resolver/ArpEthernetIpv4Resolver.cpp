/**
 * @file arpethernetipv4resolver.cpp
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

#include "ArpEthernetIpv4Resolver.h"

#include "Route.h"

namespace ProtocolLearn {
namespace Libraries {

ArpEthernetIpv4Resolver::ArpEthernetIpv4Resolver(const Ipv4Address &ipv4Address)
    : ArpEthernetIpv4Resolver(getInterfaceByIpv4Address(ipv4Address))
{
}

ArpEthernetIpv4Resolver::ArpEthernetIpv4Resolver(const Interface &interface)
    : mSocketInterface{interface},
      mEthernetStream{mSocketInterface},
      mEthernetDataStream{mEthernetStream, MacAddress{"ff:ff:ff:ff:ff:ff"}, ETH_P_ARP, interface.getMacAddress()},
      mArpStream{mEthernetDataStream},
      mArpDataStream{mArpStream, ARPHRD_ETHER, ETH_P_IP, MacAddress::MacAddressLength, Ipv4Address::Ipv4AddressLength, ARPOP_REQUEST},
      mArpEthernetIpv4Stream{mArpDataStream}
{
    // Wait 40 milliseconds.
    mArpEthernetIpv4Stream.setTimeout(PTime{0, 40000});
}

ArpEthernetIpv4Resolver::~ArpEthernetIpv4Resolver()
{
}

MacAddress ArpEthernetIpv4Resolver::resolve(const Ipv4Address &ipv4Address) {
    Arpv4::ArpEthernetIpv4Packet arpPacket;

    arpPacket.setSenderIpv4Address(mSocketInterface.getIpv4Address());
    arpPacket.setSenderMacAddress(mSocketInterface.getMacAddress());
    arpPacket.setTargetIpv4Address(ipv4Address);
    arpPacket.setTargetMacAddress(MacAddress{});

    mArpEthernetIpv4Stream.sendPacket(arpPacket);
    mArpEthernetIpv4Stream.receivePacket(arpPacket);

    return arpPacket.getSenderMacAddress();
}

Interface ArpEthernetIpv4Resolver::getInterfaceByIpv4Address(const Ipv4Address &ipv4Address) {
    const auto &routeAnswer = Route{}.getRoute(ipv4Address);

    if(routeAnswer.routeType != Route::RouteType::LocalLink)
        throw InvalidArgument("The given ipv4 address is not in the local network");

    return routeAnswer.interface;
}

} // Libraries
} // ProtcolLearns
