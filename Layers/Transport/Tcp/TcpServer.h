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

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <memory>

#include "TcpStream.h"
#include "TcpPacket.h"
#include "TcpDataStream.h"

namespace ProtocolLearn {
namespace Tcp {

class TcpServer
{
public:
    TcpServer(TcpStream &tcpStream, uint16_t ourPort);

    TcpPacket receiveSyn();

//    void sendSynAck(TcpStream &tcpStream, const TcpPacket &synPacket);

    void listen(uint16_t port);

    struct NewConnection{
        NewConnection(TcpStream &serverTcpStream, const Timeout &timeout);

        std::unique_ptr<IPProtocolStream::IPProtocolStreamFork> mIPProtocol;
        TcpStream tcpStream;
        TcpDataStream tcpDataStream;
    };

    std::unique_ptr<NewConnection> accept(const Timeout &timeout);

protected:
    TcpStream &mTcpStream;

};

}
}

#endif // TCPSERVER_H
