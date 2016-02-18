/**
 * @file main.cpp
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

#include <iostream>
#include <string>

#include "LowSocket.h"

#include "EthernetPacket.h"
#include "EthernetStream.h"

#include "ArpPacket.h"

#include "ArpEthernetIpv4Packet.h"

#include "Route.h"

void configLowSocket(ProtocolLearn::LowSocket &lowSocket, const std::string &interfaceName)
{
    lowSocket.configEthernetSocket(ProtocolLearn::Interface(interfaceName), ProtocolLearn::MacAddress());
}

int main(int argc, char *argv[]) {
    using namespace ProtocolLearn;

    using std::cout;
    using std::endl;

    if(argc != 2) {
        cout << "Usage: " << argv[0] << " <Ipv4 To Check>" << endl;
        return EXIT_FAILURE;
    }

    Ipv4Address ipv4Address(argv[1]);
    bool itIsPromicuousMode = true;

    try{
        using namespace Ethernet;
        using Arp::ArpPacket;
        using Arpv4::ArpEthernetIpv4Packet;
        using Libraries::Route;

        const auto &getRouteResults = Route().getRoute(ipv4Address);

        if(getRouteResults.gateway.toInt() != 0) {
            cout << ipv4Address.toString() << " is outside the LAN, you cannot check computer from the WAN." << endl;
            return EXIT_FAILURE;
        }

        EthernetPacket ethernetPacket;
        ethernetPacket.setDestination(MacAddress(std::string("ff:ff:ff:ff:ff:fe")));
        ethernetPacket.setSource(getRouteResults.interface.getMacAddress());

        ethernetPacket.setProtocol(ETH_P_ARP);

        ArpPacket arpPacket;
        arpPacket.setHardwareAddressFormat(ARPHRD_ETHER);
        arpPacket.setHardwareAddressLength(ETH_ALEN);
        arpPacket.setProtocolAddressFormat(ETH_P_IP);
        arpPacket.setProtocolAddressLength(Ipv4Address::Ipv4AddressLength);
        arpPacket.setOpcode(ARPOP_REQUEST);

        ArpEthernetIpv4Packet arpEthernetIpv4Packet;
        arpEthernetIpv4Packet.setSenderIpv4Address(getRouteResults.interface.getIpv4Address());
        arpEthernetIpv4Packet.setSenderMacAddress(ethernetPacket.getSource());
        arpEthernetIpv4Packet.setTargetIpv4Address(ipv4Address);
        arpEthernetIpv4Packet.setTargetMacAddress(MacAddress(std::string("00:00:00:00:00:00")));

        arpPacket.importData(arpEthernetIpv4Packet);
        ethernetPacket.importData(arpPacket);

        LowSocket lowSocket;
        configLowSocket(lowSocket, getRouteResults.interface.getName());

        EthernetStream ethernetStream(lowSocket);

        ethernetStream.setTimeout(1, 0);

        ethernetStream._send(ethernetPacket); // Send packet without filtering.
        ethernetStream.receivePacket(ethernetPacket);

    }
    catch(Timeout::TimeoutException &e) {
        // If we don't received a response.
        itIsPromicuousMode = false;
    }
    catch(ProtocolLearn::Exception &e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    cout << ipv4Address.toString() << (itIsPromicuousMode ?  " is" : " isn't") << " in Promicuous Mode." << endl;

    return EXIT_SUCCESS;
}
