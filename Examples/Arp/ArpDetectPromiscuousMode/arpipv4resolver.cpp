/**
 * @file arpipv4resolver.cpp
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

#include "arpipv4resolver.h"

#include <Route>

ArpIpv4Resolver::ArpIpv4Resolver(const Ipv4Address &ipv4Address)
{
    init(ipv4Address);
}

ArpIpv4Resolver::ArpIpv4Resolver(const Interface &interface)
{
    init(interface);
}

ArpIpv4Resolver::~ArpIpv4Resolver() {
    delete classes.ethernetStream;
    delete classes.ethernetDataStream;
    delete classes.arpStream;
    delete classes.arpDataStream;
    delete classes.arpEthernetIpv4Stream;
}

void ArpIpv4Resolver::init(const Ipv4Address &ipv4Address) {
    Route route;
    Route::GetRouteAnswer getRouteAnswer = route.getRoute(ipv4Address);

    if(getRouteAnswer.gateway.toInt() != 0)
        throw InvalidArgument("The given ipv4 address is not in the local network");

    d.interface = getRouteAnswer.interface;

    init(d.interface);
}

void ArpIpv4Resolver::init(const Interface &interface) {
    /**
     * The mac address changed in order to detect promiscuous mode.
     * For more imformation, please visit: http://www.securityfriday.com/promiscuous_detection_01.pdf.
     */
    MacAddress destination = std::string("ff:ff:ff:ff:ff:fe");

    ArpDataStream::ArpDataStreamSettings arpDataStreamSettings;
    arpDataStreamSettings.hardwareFormat = ARPHRD_ETHER;
    arpDataStreamSettings.hardwareLength = ETH_ALEN;
    arpDataStreamSettings.opcode = ARPOP_REQUEST;
    arpDataStreamSettings.protocolFormat = ETH_P_IP;
    arpDataStreamSettings.protocolLength = 4;

    LowSocket::EthernetSocketConfig ethernetSocketConfig;

    // Config the socket.
    ethernetSocketConfig.interface = interface;
    ethernetSocketConfig.macAddress = destination;
    d.socket.configEthernetSocket(ethernetSocketConfig);

    // Create ethernet stream and ethernet data stream.
    classes.ethernetStream = new EthernetStream(d.socket);
    classes.ethernetDataStream = new EthernetDataStream(*classes.ethernetStream, destination, ETH_P_ARP);

    // Create arp stream and arp data stream.
    classes.arpStream = new ArpStream(*classes.ethernetDataStream);
    classes.arpDataStream = new ArpDataStream(*classes.arpStream, arpDataStreamSettings);

    classes.arpEthernetIpv4Stream = new ArpEthernetIpv4Stream(*classes.arpDataStream, interface);

    classes.arpEthernetIpv4Stream->setTimeout(3, 0);

    classes.arpEthernetIpv4Stream->getFilter().setProtocolFilterStatus(false);

    setEthernetFilter(classes.ethernetStream->getFilter());
}

void ArpIpv4Resolver::setEthernetFilter(ProtocolFilter<EthernetPacket> &ethernetFilter) {
    EthernetFilter::FilterRuleType rule;
    EthernetFilter::FilterRulesType rules;

    // I'll to get only packet from FF:FF:FF:FF:FF:FE, but I want to get my reply packet.
    ethernetFilter.setPreviousPacketFilterStatus(false);

    // Get only Arp packets.
    rule.fieldID = EthernetFilter::Protocol;
    rule.operatorType = Equal;
    rule.value = ETH_P_ARP;

    rules.push_back(rule);

    ethernetFilter.addRules(rules);
}

MacAddress ArpIpv4Resolver::resolve(const Ipv4Address &ipv4Address) {
    MacAddress answer;

    classes.arpEthernetIpv4Stream->request(ipv4Address, answer);

    return answer;
}
