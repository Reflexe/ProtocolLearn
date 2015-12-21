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

#ifndef TCPIPV4SERVER_H
#define TCPIPV4SERVER_H

#include "TcpServer.h"

#include "TcpIpv4Stream.h"
#include "TcpIpv4Packet.h"
#include "TcpIpv4DataStream.h"

#include "SmartIpv4Stream.h"

namespace ProtocolLearn {
namespace Tcpv4 {

class TcpIpv4Server : public Tcp::TcpServer
{
public:
    TcpIpv4Server(TcpIpv4Stream &tcpStream, uint16_t ourPort);

    struct Ipv4NewConnection : NewConnection {
        Ipv4NewConnection(const Ipv4Address &ipv4Address, const Tcp::TcpFilter::TcpSession &tcpSession);

        virtual Tcp::TcpStream &getTcpStream() override;
        virtual Tcp::TcpDataStream &getTcpDataStream() override;

    private:
        Libraries::SmartIpv4Stream mSmartIpv4Stream;
        Ipv4::Ipv4DataStream mIpv4DataStream;
        TcpIpv4Stream mTcpIpv4Stream;
        TcpIpv4DataStream mTcpIpv4DataStream;
    };

    /**
     * @brief Accept a new connection.
     * @return
     */
    virtual std::unique_ptr<NewConnection> accept() override final;

private:
    TcpIpv4Stream &mTcpIpv4Stream;
};

}
}

#endif // TCPIPV4SERVER_H
