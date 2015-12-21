/**
 * @file route.cpp
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

#include "Route.h"

#include "ByteOrder.h"

namespace ProtocolLearn {

namespace Libraries {

Route::Route()
    : messageSocket(NETLINK_ROUTE), mNetlinkStream(messageSocket),
      mNetlinkDataStream(mNetlinkStream, RTM_GETROUTE, 0, NLM_F_REQUEST), mRtNetlinkStream(mNetlinkDataStream) {

    sockaddr_nl sockaddr;
    sockaddr.nl_family = AF_NETLINK;
    sockaddr.nl_groups = 0;
    sockaddr.nl_pad = 0;
    sockaddr.nl_pid = 0;

    messageSocket.getSocket().bind(SocketAddress(sockaddr));
}

Route::GetRouteAnswer Route::getRoute(const Ipv4Address &addressForRouting) {
    RtNetlink::RtNetlinkPacket rtNetlinkPacket;

    rtNetlinkPacket.setFamily(AF_INET);
    rtNetlinkPacket.setTableId(RT_TABLE_UNSPEC);
    rtNetlinkPacket.setScope(RT_SCOPE_UNIVERSE);
    rtNetlinkPacket.setProtocol(RTPROT_UNSPEC);

    // 32 bits == 4 Bytes == sizeof(Ipv4Address)
    rtNetlinkPacket.setDestinationLength(Ipv4Address::Ipv4AddressLength*8);

    rtNetlinkPacket.getParser().setOption<uint32_t>(RTA_DST, ByteOrder::hostToNetwork(addressForRouting.toInt()));

    mRtNetlinkStream.sendPacket(rtNetlinkPacket);
    mRtNetlinkStream.receivePacket(rtNetlinkPacket);

    const auto &parser = rtNetlinkPacket.getParser();
    GetRouteAnswer getRouteAnswer;

    if(parser.isOptionExist(RTA_GATEWAY)) {
        getRouteAnswer.gateway = Ipv4Address(ByteOrder::networkToHost(rtNetlinkPacket.getParser().getOption<uint32_t>(RTA_GATEWAY)));
        getRouteAnswer.routeType = RouteType::Gateway;
    } else if(rtNetlinkPacket.getType() == RTN_LOCAL)
        getRouteAnswer.routeType = RouteType::Loopback;
    else
        getRouteAnswer.routeType = RouteType::LocalLink;

    getRouteAnswer.interface = Interface(parser.getOption<Interface::InterfaceIndex>(RTA_OIF));

    return getRouteAnswer;
}

} // Libraries

} // ProtocolLearn
