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

#include "SmartIpv4Stream.h"
#include "Ipv4DataStream.h"

#include "Ipv4Address.h"

int usage(const char *progranName) {
    std::cout << "Usage: " << progranName << " <Ipv4Address>" << std::endl;
    return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
    ProtocolLearn::Debug::addDebbugedClass("Nothing");

    if(argc != 2)
        return usage(argv[0]);

    using ProtocolLearn::Libraries::SmartIpv4Stream;
    using ProtocolLearn::Ipv4Address;
    using ProtocolLearn::Ipv4::Ipv4Packet;
    using ProtocolLearn::Ipv4::Ipv4OptionsParser;
    using ProtocolLearn::Ipv4::Ipv4OptionIDType;
    using ProtocolLearn::Ipv4::Ipv4DataStream;

    using namespace ProtocolLearn::Ethernet;

    Ipv4Packet ipv4Packet;

    ipv4Packet.setDestination(Ipv4Address{argv[1]});

    Ipv4OptionsParser::OptionStructType option;
    option.optionID.optionClass = Ipv4OptionIDType::OptionClass::Debug;
    option.optionID.optionNumber = Ipv4OptionIDType::OptionNumber::Timestamp;
    option.optionID.copiedFlag = true;

    std::string str = "ProtocolLearn is awesome!";

    option.data.insert(option.data.begin(), str.cbegin(), str.cend());
    auto keyID = option.optionID;
    keyID.copiedFlag = false;

    ipv4Packet.getParser().setOption(keyID.toOctet(), option);

    ProtocolLearn::OctetVector octetVector{str.cbegin(), str.cend()};

    for(uint i = 0; i < 3500/str.size()+0xf; ++i)
    {
        octetVector.insert(octetVector.end(), str.cbegin(), str.cend());
    }
    ipv4Packet.importData(std::move(octetVector));

    SmartIpv4Stream smartIpv4Stream{ipv4Packet.getDestination()};

    smartIpv4Stream.getIpv4Stream().sendPacket(ipv4Packet);

    return 0;
}
