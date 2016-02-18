/**
 * @file lowsocket.h
 * @author shrek0 (shrek0.tk@gmail.com)
  LowSocket
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
 * Provides a class for Low-Level sockets input/output.
 */

#ifndef LOWSOCKET_H
#define LOWSOCKET_H

#include <vector>

/* AF_PACKET */
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if_arp.h>

#include "Socket.h"
#include "Interface.h"

#include "MacAddress.h"

#include "SocketDataStream.h"

namespace ProtocolLearn {

class LowSocket : public SocketDataStream
{
public:
    explicit LowSocket(int type=SOCK_RAW, int protocol=0x300, int domain=AF_PACKET);

    explicit LowSocket(const Interface &interface, const MacAddress &macAddress, int type=SOCK_RAW, int protocol=0x300, int domain=AF_PACKET);

    struct Config{
        Interface interface;
        MacAddress macAddress;
    };
    explicit LowSocket(const Config &config);

    void setTimeout(uint seconds, useconds_t useconds)
    {
        getSocket().setTimeout(seconds, useconds);
    }

    void configEthernetSocket(const Interface &interface, const MacAddress &macAddress);

    void setPromiscuousMode(bool status);

    const Interface &getInterface() const{ return mInterface; }
    const MacAddress &getMacAddress() const{ return mMacAddress; }

private:
    Interface mInterface;
    MacAddress mMacAddress;
};

} // ProtocolLearn

#endif // LOWSOCKET_H
