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

#include "TcpIpv4Server.h"

namespace ProtocolLearn {
namespace Tcpv4 {

TcpIpv4Server::TcpIpv4Server(TcpIpv4Stream &tcpStream, uint16_t ourPort)
    : Tcp::TcpServer{tcpStream, ourPort}, mTcpIpv4Stream{tcpStream}
{
}

std::unique_ptr<Tcp::TcpServer::NewConnection> TcpIpv4Server::accept() {
    receiveSyn();

    pl_debug("SYN received");

    std::unique_ptr<NewConnection> pNewConnection{new Ipv4NewConnection{mTcpIpv4Stream.getIpv4DataStream().getReceivePacket().getSource(),
                    mTcpStream.getFilter().getSession()}};
    mTcpIpv4Stream.setTcpState(Tcp::TcpFilter::Listen);

    return std::move(pNewConnection);
}

TcpIpv4Server::Ipv4NewConnection::Ipv4NewConnection(const Ipv4Address &ipv4Address, const Tcp::TcpFilter::TcpSession &tcpSession)
    : mSmartIpv4Stream{ipv4Address},
      mIpv4DataStream{mSmartIpv4Stream.getIpv4Stream(), ipv4Address, IPPROTO_TCP, mSmartIpv4Stream.getInterface().getIpv4Address()},
      mTcpIpv4Stream{mIpv4DataStream},
      mTcpIpv4DataStream{TcpIpv4DataStream::completeAccepting(mTcpIpv4Stream, tcpSession)}
{
}

Tcp::TcpStream &TcpIpv4Server::Ipv4NewConnection::getTcpStream()
{
    return mTcpIpv4Stream;
}

Tcp::TcpDataStream &TcpIpv4Server::Ipv4NewConnection::getTcpDataStream()
{
    return mTcpIpv4DataStream;
}

}
}
