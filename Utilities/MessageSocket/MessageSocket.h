/**
 * @file messagesocket.h
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

#ifndef MESSAGESOCKET_H
#define MESSAGESOCKET_H

#include "Socket.h"
#include "DataStream.h"

namespace ProtocolLearn {

class MessageSocket : public DataStream
{
public:
    explicit MessageSocket(int protocol);

    void sendMessage(const OctetVector &data) const;
    void receiveMessage(::std::vector<uint8_t> &data) const;

    size_t recvmsg(msghdr &msgHeader) const;
    size_t sendmsg(const msghdr &msgHeader) const;

    virtual OctetVector _recv(const Timeout &timeout) override;
    virtual void _send(OctetVector &&data) override;

    void setTimeout(const Timeout::TimeType &timeout);

    Socket &getSocket();

private:
    PTime mCurrentTimeout{PTime::infinity()};

    Socket mSocket;
};

} // ProtocolLearn

#endif // MESSAGESOCKET_H
