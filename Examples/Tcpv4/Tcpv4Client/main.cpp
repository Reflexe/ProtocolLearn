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

int main(int argc, char *argv[]) {
    if(argc != 3)
        return EXIT_FAILURE;

    ProtocolLearn::Debug::addDebbugedClass("Protocols/Tcp");
    ProtocolLearn::Debug::addDebbugedClass("Examples/Tcpv4/Tcpv4Client");

    using ProtocolLearn::Interface;
    using ProtocolLearn::MacAddress;
    using ProtocolLearn::Ipv4Address;

    using namespace ProtocolLearn::Ipv4;
    using namespace ProtocolLearn::Tcp;

    ProtocolLearn::Libraries::SmartIpv4Stream smartIpv4Stream{Ipv4Address{argv[1]}};

    Ipv4DataStream ipv4DataStream{smartIpv4Stream.getIpv4Stream(), Ipv4Address{argv[1]}, IPPROTO_TCP,
                smartIpv4Stream.getSocketInterface().getIpv4Address()};

    IPProtocolIpv4 ipProtocolIpv4{ipv4DataStream};

    TcpStream tcpIpv4Stream{ipProtocolIpv4};
    auto tcpIpv4DataStream = TcpDataStream::connect(tcpIpv4Stream,
                                                    ProtocolLearn::Convertions::toInteger<uint16_t>(std::string(argv[2])));

    {
        std::string message = "DigitalWhisper!";
        ProtocolLearn::OctetVector octetVector{message.begin(), message.end()};

        tcpIpv4DataStream.sendData(std::move(octetVector));
        tcpIpv4DataStream.sync();
    }

    tcpIpv4DataStream.close();

//    tcpIpv4DataStream.sendData();

    return 0;
}
