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

#include <EthernetStream.h>
#include <EthernetDataStream.h>

#include <iostream>

int main(int, char *[]) {
    using namespace ProtocolLearn;
    using namespace Ethernet;

//    try{
//        Interface interface("lo");
//        MacAddress destination("ff:ff:ff:ff:ff:ff");
//        MacAddress source("12:34:56:78:91:0F");
//        LowSocket socket;
//        EthernetStream *ethernetStream;
//        EthernetDataStream *ethernetDataStream;

//        Packet::DataType data;
//        std::string message = "Hello World!";

//        data.insert(data.begin(), message.begin(), message.end());

//        socket.configEthernetSocket(interface, destination);

//        std::cout << "Creating EthernetStream..." << std::endl;
//        ethernetStream = new EthernetStream(socket);

//        std::cout << "Creating EthernetDataStream..." << std::endl;
//        ethernetDataStream = new EthernetDataStream(*ethernetStream, destination, ETH_P_IP, source);

//        std::cout << "Sending data..." << std::endl;
//        ethernetDataStream->sendData(data);

//        delete ethernetStream;
//        delete ethernetDataStream;
//    }
//    catch(Exception &e)
//    {
//        std::cout << e.what() << std::endl;
//    }

    return 0;
}
