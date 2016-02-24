/**
 * @file messagesocket.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
  MessageSocket
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

#include "MessageSocket.h"

namespace ProtocolLearn {

size_t MessageSocket::sendmsg(const msghdr &msgHeader) const{
    ssize_t stats = ::sendmsg(mSocket.getFileDescriptor(), &msgHeader, 0);

    if(stats < 0)
        throw Socket::SocketException("sendmsg");

    return static_cast<size_t>(stats);
}

size_t MessageSocket::recvmsg(msghdr &msgHeader) const{
    ssize_t stats = ::recvmsg(mSocket.getFileDescriptor(), &msgHeader, 0);

    if(stats < 0)
        throw Socket::SocketException("recvmsg");

    return static_cast<size_t>(stats);
}

MessageSocket::MessageSocket(int protocol)
    : mSocket(AF_NETLINK, SOCK_RAW, protocol)
{
}

void MessageSocket::sendMessage(const OctetVector &data) const{
    msghdr msgHeader;
    iovec iove;

    msgHeader.msg_control = 0;
    msgHeader.msg_controllen = 0;
    msgHeader.msg_flags = 0;
    msgHeader.msg_iov = &iove;
    msgHeader.msg_iovlen = 1;
    msgHeader.msg_name = const_cast<sockaddr_nl *>(mSocket.getSocketAddress().getSockaddr<sockaddr_nl>());
    msgHeader.msg_namelen = mSocket.getSocketAddress().getLength();

    iove.iov_base = const_cast<uint8_t *>(data.data());
    iove.iov_len = data.size();

    sendmsg(msgHeader);
}

void MessageSocket::receiveMessage(std::vector<uint8_t> &data) const{
    msghdr msgHeader;
    iovec iove;

    msgHeader.msg_control = 0;
    msgHeader.msg_controllen = 0;
    msgHeader.msg_flags = 0;
    msgHeader.msg_name = 0;
    msgHeader.msg_namelen = 0;

    msgHeader.msg_iovlen = 1;

    msgHeader.msg_iov = &iove;
    msgHeader.msg_iovlen = 1;

    data.resize(16384);

    iove.iov_base = data.data();
    iove.iov_len = data.size();

    data.resize(recvmsg(msgHeader));
}

OctetVector MessageSocket::_recv() {
    OctetVector data;
    receiveMessage(data);
    return data;
}

void MessageSocket::_send(OctetVector &&data)
{
    sendMessage(data);
}

void MessageSocket::setTimeout(const Timeout::TimeType &timeout) {
    mSocket.setTimeout(timeout);
    DataStream::setTimeout(timeout);
}

Socket &MessageSocket::getSocket()
{
    return mSocket;
}

}
