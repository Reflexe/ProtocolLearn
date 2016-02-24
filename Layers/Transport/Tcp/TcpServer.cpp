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

#include "TcpServer.h"

namespace ProtocolLearn {
namespace Tcp {

TcpServer::TcpServer(TcpStream &tcpStream, uint16_t ourPort)
    : mTcpStream{tcpStream}
{
    listen(ourPort);
}

TcpPacket TcpServer::receiveSyn() {
    TcpPacket receivePacket;

    mTcpStream.receivePacket(receivePacket);

    return receivePacket;
}

void TcpServer::listen(uint16_t port) {
    mTcpStream.setTimeout(PTime::infinity()); // We can wait forever :)
    mTcpStream.getTcpSession().our.port = port;
    mTcpStream.setTcpState(TcpFilter::Listen);
}

std::unique_ptr<TcpServer::NewConnection> TcpServer::accept() {
    pl_debug("ReceiveSYN!!");
    receiveSyn();
    pl_debug("received!");

    return std::unique_ptr<TcpServer::NewConnection>{new NewConnection{mTcpStream}};
}

TcpServer::NewConnection::NewConnection(TcpStream &serverTcpStream)
    : mIPProtocol{serverTcpStream.getPacketStream().fork(true)},
      tcpStream{mIPProtocol->getIPProtocol()},
      tcpDataStream{TcpDataStream::completeAccept(tcpStream, serverTcpStream.getTcpSession())}
{
    serverTcpStream.setTcpState(TcpFilter::Listen);
}

}
}
