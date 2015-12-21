/**
 * @file %{FILENAME}
 * @author shrek0 (shrek0.tk@gmail.com)
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) %YEAR% shrek0
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

#ifndef PROTOCOLLEARN_SOCKETINTERFACE_H
#define PROTOCOLLEARN_SOCKETINTERFACE_H

#include "BasicInterface.h"
#include "Interface.h"

#include "Socket.h"
#include "ByteOrder.h"

namespace ProtocolLearn {

class SocketInterface : public BasicInterface
{
public:
    SocketInterface(const Interface &interface);

    virtual void _recv(OctetVector &data) override;
    virtual void _send(const OctetVector &data) override;

    void setPromiscuousMode(bool status);

    virtual MacAddress getMacAddress() override;

    virtual Ipv4Address getIpv4Address() override;
    virtual Ipv4Address getIpv4NetworkMask() override;

    virtual Ipv6Address getIpv6Address() override;
    virtual Ipv6Address getIpv6NetworkMask() override;

    virtual OctetVector::SizeType getMaximumSendDataLength() override;

    virtual std::unique_ptr<BasicInterfaceFork> fork() override;

private:
    Socket mSocket{AF_PACKET, SOCK_RAW, ByteOrder::hostToNetwork(static_cast<uint16_t>(ETH_P_ALL))};
    Interface mInterface;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_SOCKETINTERFACE_H
