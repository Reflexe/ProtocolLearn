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

#include "SmartIpv4Stream.h"

#include "Route.h"
#include "ArpEthernetIpv4Resolver.h"

namespace ProtocolLearn {
namespace Libraries {

SmartIpv4Stream::SmartIpv4Stream(const Ipv4Address &destionation)
    : SmartIpv4Stream{generateLowSocketConfig(destionation)}
{
}

SmartIpv4Stream::SmartIpv4Stream(const std::pair<Interface, MacAddress> &interfaceAndAddress)
    : mSocketInterface{interfaceAndAddress.first},
      mEthernetStream{mSocketInterface},
      mEthernetDataStream{mEthernetStream, interfaceAndAddress.second, ETH_P_IP, mSocketInterface.getMacAddress()},
      mIpv4Stream{mEthernetDataStream}
{
}

std::pair<Interface,MacAddress> SmartIpv4Stream::generateLowSocketConfig(const Ipv4Address &ipv4Address) {
    const auto &routeAnswer = Route{}.getRoute(ipv4Address);
    std::pair<Interface,MacAddress> lowSocketConfig;

    lowSocketConfig.first = routeAnswer.interface;

    if(routeAnswer.routeType == Route::RouteType::Loopback) {
        lowSocketConfig.second = routeAnswer.interface.getMacAddress();
    } else if(routeAnswer.routeType == Route::RouteType::Gateway || routeAnswer.routeType == Route::RouteType::LocalLink) {

        // If it's a local address, get its Mac Address, if not, get the gateway's Mac Address.
        const Ipv4Address &ipv4AddressToResolve = routeAnswer.routeType == Route::RouteType::LocalLink ? ipv4Address : routeAnswer.gateway;

        lowSocketConfig.second = ArpEthernetIpv4Resolver{routeAnswer.interface}.resolve(ipv4AddressToResolve);
    }

    return lowSocketConfig;
}

Ethernet::EthernetDataStream &SmartIpv4Stream::getEthernetDataStream()
{
    return mEthernetDataStream;
}

Ethernet::EthernetStream &SmartIpv4Stream::getEthernetStream()
{
    return mEthernetStream;
}

} // namespace Libraries
} // namespace ProtocolLearn

