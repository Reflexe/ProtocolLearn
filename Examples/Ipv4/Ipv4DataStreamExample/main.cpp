/**
 * @file main.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
 * @class
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

#include <EthernetDataStream.h>
#include <Ipv4DataStream.h>

#include <Route.h>
#include <ArpEthernetIpv4Resolver.h>

#include <iostream>

int usage(const char *pProgramName) {
    using std::cout;
    using std::endl;

    cout << "Usage: " << pProgramName << "<Ipv4 Address> <Message>" << endl;

    return -1;
}

int main(int argc, char *argv[]) {
    if(argc != 3)
        return usage(argv[0]);

    using namespace ProtocolLearn;

    using Libraries::Route;
    using Libraries::ArpEthernetIpv4Resolver;

    Ipv4Address destination(argv[1]);
    ProtocolLearn::OctetVector data;
    std::string message = argv[2];

    data.insert(data.begin(), message.begin(), message.end());

    std::cout << "Getting Route..";

    const auto &getRouteAnswer = Route().getRoute(destination);

    std::cout << "Done. Interface:" << getRouteAnswer.interface.getName() << std::endl;

    MacAddress macAddress;

    try{
        ArpEthernetIpv4Resolver resolver(getRouteAnswer.interface);

        // If this ipv4 is on our LAN network, get its mac address. If not, get the gateway's mac address.
        if(getRouteAnswer.gateway == 0)
            macAddress = resolver.resolve(destination);
        else
            macAddress = resolver.resolve(getRouteAnswer.gateway);
    }
    catch(Exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    try{
        using namespace Ethernet;
        using namespace Ipv4;

        LowSocket lowSocket;

        lowSocket.configEthernetSocket(getRouteAnswer.interface, macAddress);

        EthernetStream ethernetStream(lowSocket);
        EthernetDataStream ethernetDataStream(ethernetStream, lowSocket.getMacAddress(), ETH_P_IP, getRouteAnswer.interface.getMacAddress());

        Ipv4Stream ipv4Stream(ethernetDataStream, lowSocket.getInterface().getMTU());
        Ipv4DataStream ipv4DataStream(ipv4Stream, destination, IPPROTO_TCP, getRouteAnswer.interface.getIpv4Address());

        ipv4DataStream.sendData(data);
    }
    catch(Exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cout << "Done!" << std::endl;
}
