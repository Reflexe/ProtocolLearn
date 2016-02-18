/**
 * @file socket.h
 * @author shrek0 (shrek0.tk@gmail.com)
  Socket
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
 * Contain a class that provide basic socket input/output
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>

#include "OctetVector.h"
#include "SocketAddress.h"
#include "ErrnoException.h"
#include "Timeout.h"

#include <cerrno>
#include <cstring>

#include <unistd.h>

#include <list>

namespace ProtocolLearn {

class Socket
{
public:
    typedef int FileDescriptorType;

    Socket(int domain=AF_INET, int type=SOCK_STREAM, int protocol=0);

    Socket(Socket &&) = default;
    Socket(const Socket &) = delete;

    virtual ~Socket();

    Socket &operator =(Socket &&socket);

    /* You can't have two sockets that receive from the same OS socket */
    Socket &operator =(const Socket &socket) = delete;

    void close();
    bool isOpen() const;

    void setTimeout(const Timeout::TimeType &time);

    void setSocketOption(int level, int optionName, const void *option, socklen_t optionLength);

    void setSockaddr(const SocketAddress &socketAddress);

    void send(const OctetVector &buffer, OctetVector::size_type packetSize=0, int flags=0) const;
    void receive(OctetVector &buffer, OctetVector::size_type maxPacketSize=0, int flags=0) const;

    void receiveFrom(OctetVector &buffer, OctetVector::size_type maxPacketSize);
    void sendTo(const OctetVector &buffer, const SocketAddress &socketAddress) const;

    void sendTo(const OctetVector &buffer) const
    {
        sendTo(buffer, s.socketAddress);
    }

    FileDescriptorType getFileDescriptor() const{ return s.fileDescriptor; }

    void bind(const SocketAddress &socketAddress);
//    std::list<Socket> select(const std::list<Socket> &sockets);

    const SocketAddress &getSocketAddress() const{ return s.socketAddress; }

    /* Exceptions */

    class SocketException: public ErrnoException
    {
    public:
        using ErrnoException::ErrnoException;
    };

private:
    void setSocketTimeout(const timeval &timeval);

    struct SocketData{
        SocketAddress socketAddress;
        FileDescriptorType fileDescriptor;
    };

    SocketData s;
};

} // ProtocolLearn

#endif // SOCKET_H
