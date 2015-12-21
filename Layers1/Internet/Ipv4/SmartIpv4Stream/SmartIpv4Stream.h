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

#ifndef PROTOCOLLEARN_LIBRARIES_SMARTIPV4STREAM_H
#define PROTOCOLLEARN_LIBRARIES_SMARTIPV4STREAM_H

#include "SocketInterface.h"
#include "EthernetStream.h"
#include "EthernetDataStream.h"

#include "Ipv4Stream.h"

namespace ProtocolLearn {
namespace Libraries {

/**
 * @brief The SmartIpv4Stream class  A Ipv4 stream that will imploment the Route and the Two Layer by itself.
 */
class SmartIpv4Stream
{
public:
    /**
     * @brief SmartIpv4Stream  A Client constructor.
     * @param destionation
     */
    SmartIpv4Stream(const Ipv4Address &destionation);

    /**
     * @brief SmartIpv4Stream  A server constructor.
     * @param interface
     */
    SmartIpv4Stream(const std::pair<Interface, MacAddress> &interfaceAndAddress);

    Ipv4::Ipv4Stream &getIpv4Stream() { return mIpv4Stream; }
    const Ipv4::Ipv4Stream &getIpv4Stream() const{ return mIpv4Stream; }

    const SocketInterface &getSocketInterface() const{ return mSocketInterface; }
    SocketInterface &getSocketInterface() { return mSocketInterface; }

    Ethernet::EthernetStream &getEthernetStream();

    Ethernet::EthernetDataStream &getEthernetDataStream();

private:
//    SmartIpv4Stream(const LowSocket::Config &config);
    std::pair<Interface,MacAddress> generateLowSocketConfig(const Ipv4Address &ipv4Address);

    SocketInterface mSocketInterface;

    Ethernet::EthernetStream mEthernetStream;
    Ethernet::EthernetDataStream mEthernetDataStream;

    Ipv4::Ipv4Stream mIpv4Stream;
};

} // namespace Libraries
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_LIBRARIES_SMARTIPV4STREAM_H
