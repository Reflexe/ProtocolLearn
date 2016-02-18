/**
 * @file socket.cpp
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

#include "Socket.h"

/* AF_INET */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

namespace ProtocolLearn {

Socket::Socket(int domain, int type, int protocol) {
    s.fileDescriptor = ::socket(domain, type, protocol);

    if(s.fileDescriptor < 0)
        throw SocketException("socket()");
}

Socket::~Socket()
{
    close();
}

Socket &Socket::operator =(Socket &&socket) {
    close(); // First, close the open socket.

    s = std::move(socket.s);

    return *this;
}

void Socket::close() {
    if(isOpen() == false)
        return;

    ::close(s.fileDescriptor);
    s.fileDescriptor=0;
}

bool Socket::isOpen() const
{
    return s.fileDescriptor != 0;
}

void Socket::setTimeout(const Timeout::TimeType &time) {
    timeval timeout;

    if(time.isInfinty()) {
        timeout.tv_sec = timeout.tv_usec = 0;
    } else if(time.getMicroSeconds() == 0 && time.getSeconds() == 0) {
        int flags = fcntl(getFileDescriptor(), F_GETFL);
        ::fcntl(getFileDescriptor(), F_SETFL, flags | O_NONBLOCK);

        return;
    } else {
        timeout.tv_sec = time.getSeconds();
        timeout.tv_usec = time.getMicroSeconds();
    }

    setSocketTimeout(timeout);
}

void Socket::receive(OctetVector &buffer, OctetVector::size_type maxPacketSize, int flags) const{
    ssize_t stats = 0;
    std::vector<uint8_t>::size_type reveiveSize = maxPacketSize == 0 ? buffer.size() : maxPacketSize;

    buffer.resize(reveiveSize);

    stats = ::recv(s.fileDescriptor, buffer.data(),
                   reveiveSize,
                   flags);

    if(stats < 0)
        throw SocketException("Socket::recv()");

    buffer.resize(stats);
}

void Socket::send(const OctetVector &buffer, OctetVector::size_type packetSize, int flags) const{
    ssize_t stats = 0;

    stats = ::send(s.fileDescriptor, buffer.data(), (packetSize == 0 ? buffer.size() : packetSize), flags);

    if(stats < 0)
        throw SocketException("send()");
}

void Socket::receiveFrom(OctetVector &buffer, OctetVector::size_type maxPacketSize){
    ssize_t stats = 0;
    OctetVector::size_type reveiveSize = maxPacketSize == 0 ? buffer.size() : maxPacketSize;

    buffer.resize(reveiveSize);

    stats = ::recvfrom(s.fileDescriptor, buffer.data(), buffer.size(), 0, nullptr, nullptr);

    if(stats < 0)
        throw Socket::SocketException("recvfrom()");

    buffer.resize(static_cast<std::vector<uint8_t>::size_type>(stats));
}

void Socket::sendTo(const OctetVector &buffer, const SocketAddress &socketAddress) const{
    ssize_t stats = 0;

    stats = ::sendto(s.fileDescriptor, buffer.data(), buffer.size(), 0, socketAddress.getSockaddr<sockaddr>(), socketAddress.getLength());

    if(stats < 0)
        throw Socket::SocketException("sendto()");
}

void Socket::setSockaddr(const SocketAddress &socketAddress)
{
    s.socketAddress = socketAddress;
}

void Socket::setSocketOption(int level, int optionName, const void *option, socklen_t optionLength) {
    ssize_t stats = ::setsockopt(s.fileDescriptor, level, optionName, option, optionLength);

    if(stats < 0)
        throw SocketException("setsocketopt()");
}

void Socket::bind(const SocketAddress &socketAddress) {
    if(::bind(s.fileDescriptor, socketAddress.getSockaddr<sockaddr>(), socketAddress.getLength()) < 0)
        throw SocketException("Cannot Bind!");

    setSockaddr(socketAddress);
}

void Socket::setSocketTimeout(const timeval &timeval) {
    setSocketOption(SOL_SOCKET, SO_RCVTIMEO, &timeval, sizeof(timeval));
    setSocketOption(SOL_SOCKET, SO_SNDTIMEO, &timeval, sizeof(timeval));
}

//std::list<Socket> Socket::select(const std::list<Socket> &sockets) {
//    fd_set fdSet;

//    for(const auto &socket : sockets)
//    {
//        FD_SET(fdSet, socket.getFD());
//    }

//    fd_set readfs;
//    ::select(sockets.size(), &readfs, 0, 0, 0);

//   std::list<Socket> readSockets;
//   for(const auto &socket : sockets) {
//       if(FD_ISSET(readfs, socket.getFD()))
//           readSockets.push_back(socket);
//   }

//   return readSockets;
//}

} // ProtocolLearn
