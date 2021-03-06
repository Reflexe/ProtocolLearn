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

#include <iostream>

#include "SmartIpv4Stream.h"

#include "Ipv4DataStream.h"

#include "IcmpStream.h"
#include "IcmpDataStream.h"

#include "Random.h"

#include <unistd.h>

int usage(const char *programName);
ProtocolLearn::Ipv4::Ipv4Filter::DropReasonType ipv4FilterFunction(const ProtocolLearn::Ipv4::Ipv4Packet &ipv4Packet);

int usage(const char *programName) {
    std::cout << "Usage: " << programName << " [Ipv4 Address] <Data>" << std::endl;

    return EXIT_FAILURE;
}

ProtocolLearn::Ipv4::Ipv4Filter::DropReasonType ipv4FilterFunction(const ProtocolLearn::Ipv4::Ipv4Packet &ipv4Packet) {
    using ProtocolLearn::Ipv4::Ipv4Filter;

    return ipv4Packet.getProtocol() == IPPROTO_ICMP ? Ipv4Filter::DropReason::None : Ipv4Filter::DropReason::NoReason;
}

int main(int argc, char *argv[]) {
    if(argc < 2 || argc > 3)
        return usage(argv[0]);

    using ProtocolLearn::Ipv4Address;

    using ProtocolLearn::Libraries::SmartIpv4Stream;
    using ProtocolLearn::Ipv4::Ipv4DataStream;

    using ProtocolLearn::Icmp::IcmpStream;
    using ProtocolLearn::Icmp::IcmpPacket;

    using ProtocolLearn::Random;

    using ProtocolLearn::PTime;

    Ipv4Address destination{argv[1]};

    SmartIpv4Stream smartIpv4Stream{destination};
    smartIpv4Stream.getIpv4Stream().getFilter().setUserFilterStatus(true, ipv4FilterFunction);
    smartIpv4Stream.getIpv4Stream().getFilter().setPreviousPacketFilterStatus(false);

    Ipv4DataStream ipv4DataStream{smartIpv4Stream.getIpv4Stream(), destination, IPPROTO_ICMP,
                smartIpv4Stream.getSocketInterface().getIpv4Address()};

    IcmpStream icmpStream{ipv4DataStream};

    IcmpPacket icmpPacket;
    icmpPacket.setSequence(static_cast<uint16_t>(Random::getMediumRandomNumber()));
    icmpPacket.setId(static_cast<uint16_t>(Random::getMediumRandomNumber()));
    icmpPacket.setType(ICMP_ECHO);
    icmpPacket.setCode(0);

    {
        std::string dataString = (argc == 2 ? "Do you want a cookie?" : argv[1]);

        ProtocolLearn::OctetVector data{dataString.cbegin(), dataString.cend()};

        icmpPacket.importData(data);
    }

    IcmpPacket receivedPacket;

    std::cout << "Sending a ping to " << destination.toString() << std::endl;

    PTime ptime = PTime::now();

    icmpStream.sendPacket(icmpPacket);

    try
    {
        icmpStream.receivePacket(receivedPacket, PTime{2, 0});
    }
    catch(ProtocolLearn::Timeout::TimeoutException &){
        std::cout << "** No Answer." << std::endl;
        return EXIT_FAILURE;
    }

    if(receivedPacket.getType() != ICMP_ECHOREPLY) {
        std::cerr << "Error: invalid type received." << std::endl;
        return EXIT_FAILURE;
    }

    ptime = PTime::now()-ptime;

    std::cout << "Answer from " << ipv4DataStream.getReceivePacket().getSource().toString()
              << " in " << ptime.getSeconds() << " seconds" << " and " << ptime.getMicroSeconds() << " micro seconds" << std::endl;

    return EXIT_SUCCESS;
}
