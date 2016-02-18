/**
 * @file SocketDataStream.h
 * @author shrek0 (shrek0.tk@gmail.com)
  SocketDataStream
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

#ifndef SOCKETDATASTREAM_H
#define SOCKETDATASTREAM_H

#include "Socket.h"
#include "DataStream.h"

namespace ProtocolLearn {

class SocketDataStream : public DataStream
{
public:
    SocketDataStream(int domain=AF_INET, int type=SOCK_STREAM, int protocol=0);

    void bind(const SocketAddress &socketAddress);

    virtual void _recv(OctetVector &data) override;
    virtual void _send(const OctetVector &data) override;

    virtual void setTimeout(const Timeout::TimeType &time) override;

    Socket &getSocket() { return mSocket; }
    const Socket &getSocket() const{ return mSocket; }

private:
    /**
     * @brief socket  The socket to read and write to.
     */
    Socket mSocket;
};

}

#endif // SOCKETDATASTREAM_H
