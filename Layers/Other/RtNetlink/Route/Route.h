/**
 * @file route.h
 * @author shrek0 (shrek0.tk@gmail.com)
  Route
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

#ifndef ROUTE_H
#define ROUTE_H

#include "Interface.h"
#include "Ipv4Address.h"

#include "MessageSocket.h"

#include "NetlinkStream.h"
#include "NetlinkDataStream.h"

#include "RtNetlinkStream.h"

namespace ProtocolLearn {

namespace Libraries {

class Route
{
public:
    Route();

    enum class RouteType{
        Loopback,     /* On the same comuter */
        LocalLink,    /* On the same Network */
        Gateway       /* On other Network  */
    };

    struct GetRouteAnswer{
        Interface interface;
        Ipv4Address gateway; //When an local routing, gateway value will be set to zero.
        RouteType routeType;
    };

    class RouteNotFound : public RuntimeError
    {
        using RuntimeError::RuntimeError;
    };

    /**
     * @brief Route::getRoute  Request for route from the kernel.
     * @param addressForRouting  Ipv4 Address for the routing.
     * @return the Address's Interface and its Gateway.
     */
    GetRouteAnswer getRoute(const Ipv4Address &addressForRouting);

private:
    MessageSocket messageSocket;

    Netlink::NetlinkStream mNetlinkStream;
    Netlink::NetlinkDataStream mNetlinkDataStream;
    RtNetlink::RtNetlinkStream mRtNetlinkStream;
};

} // Libraries

} // ProtcolLearn

#endif // ROUTE_H
