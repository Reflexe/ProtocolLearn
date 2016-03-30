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

#include "Ipv4DataStream.h"
#include "IPProtocolIpv4.h"
#include "TcpServer.h"

ProtocolLearn::Ethernet::EthernetFilter::DropReasonType ethernetFilterCallback(const ProtocolLearn::Ethernet::EthernetPacket &filteredPacket);
ProtocolLearn::Ipv4::Ipv4Filter::DropReasonType ipv4FilterCallback(const ProtocolLearn::Ipv4::Ipv4Packet &ipv4Packet);

ProtocolLearn::Ethernet::EthernetFilter::DropReasonType ethernetFilterCallback(const ProtocolLearn::Ethernet::EthernetPacket &filteredPacket) {
    using ProtocolLearn::Ethernet::EthernetFilter;

    return filteredPacket.getProtocol() == ETH_P_IP ? EthernetFilter::DropReason::None : EthernetFilter::DropReason::NoReason;
}

ProtocolLearn::Ipv4::Ipv4Filter::DropReasonType ipv4FilterCallback(const ProtocolLearn::Ipv4::Ipv4Packet &ipv4Packet) {
    using ProtocolLearn::Ipv4::Ipv4Filter;

    return ipv4Packet.getProtocol() == IPPROTO_TCP ? Ipv4Filter::DropReason::None : Ipv4Filter::DropReason::NoReason;
}

int main(int , char *argv[]) {
    ProtocolLearn::Debug::addDebbugedClass("Layers/Transport/Tcp");
    ProtocolLearn::Debug::addDebbugedClass("Layers/Internet/Ipv4");
    ProtocolLearn::Debug::addDebbugedClass("Utilities/TcpIpv4OptionsParser");
//    ProtocolLearn::Debug::addDebbugedClass("Utilities/ProtocolFilter");
//    ProtocolLearn::Debug::addDebbugedClass("Utilities/PacketStream");
    ProtocolLearn::Debug::addDebbugedClass("Examples/Tcpv4/Tcpv4Server");
//    ProtocolLearn::Debug::addDebbugedClass("Nothing");

    using ProtocolLearn::Interface;
    using ProtocolLearn::MacAddress;
    using ProtocolLearn::Ipv4Address;

    using namespace ProtocolLearn::Ipv4;
    using namespace ProtocolLearn::Tcp;

    ProtocolLearn::Libraries::SmartIpv4Stream smartIpv4Stream{{Interface{argv[1]}, MacAddress{}}};
    smartIpv4Stream.getEthernetStream().getFilter().setPreviousPacketFilterStatus(false);
    smartIpv4Stream.getEthernetStream().getFilter().setUserFilterStatus(true, ethernetFilterCallback);

    smartIpv4Stream.getIpv4Stream().getFilter().setPreviousPacketFilterStatus(false);
    smartIpv4Stream.getIpv4Stream().getFilter().setUserFilterStatus(true, ipv4FilterCallback);

    Ipv4DataStream ipv4DataStream{smartIpv4Stream.getIpv4Stream(), Ipv4Address{}, IPPROTO_TCP,
                smartIpv4Stream.getSocketInterface().getIpv4Address()};

    IPProtocolIpv4 ipProtocolIpv4{ipv4DataStream};

    TcpStream tcpIpv4Stream{ipProtocolIpv4};
    TcpServer tcpIpv4Server{tcpIpv4Stream, 9090};

    pl_debug("Waiting for connection!");

    auto newConnection = tcpIpv4Server.accept(ProtocolLearn::PTime::infinity());
    auto &tcpDataStream = newConnection->tcpDataStream;

    pl_debug("Accepted! from: " << ipv4DataStream.getReceivePacket().getSource().toString());

    ProtocolLearn::OctetVector octetVector;

//    tcpDataStream.setMinimumReceiveDataSize(0);

    while(tcpDataStream.isConnected()) {
        pl_debug("Receiving data..");
        try {
            octetVector = tcpDataStream.receiveData(ProtocolLearn::PTime::infinity());
        } catch(ProtocolLearn::Tcp::TcpDataStream::NotConnectedStream &) {
            break;
        }

        {
            std::string str{octetVector.begin(), octetVector.end()};
            pl_debug("Data received: " << str);
        }

        pl_debug("Sending data back..");
        tcpDataStream.sendData(std::move(octetVector));
//        tcpDataStream.sync();
    }

    pl_debug("Done.");
    tcpDataStream.close(ProtocolLearn::PTime::infinity());

    return 0;
}
