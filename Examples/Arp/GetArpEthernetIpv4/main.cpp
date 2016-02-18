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

#include "ArpEthernetIpv4Resolver.h"

int main(int argc, char *argv[]) {
    using namespace ProtocolLearn;
    using Libraries::ArpEthernetIpv4Resolver;

    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " <IPv4 address to resolve>" << std::endl;
        return -1;
    }

    Ipv4Address ipv4Address(argv[1]);

    try{
        ArpEthernetIpv4Resolver ipv4EthernetIpv4Resolver(ipv4Address);
        MacAddress result = ipv4EthernetIpv4Resolver.resolve(ipv4Address);

        std::cout << result.toString() << std::endl;
    }
    catch(Timeout::TimeoutException &)
    {
        std::cout << "Not found :(" << std::endl;
    }
    catch(Exception &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
}
