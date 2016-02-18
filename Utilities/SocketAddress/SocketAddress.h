/**
 * @file socketaddress.h
 * @author shrek0 (shrek0.tk@gmail.com)
  SocketAddress
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

#ifndef SOCKETADDRESS_H
#define SOCKETADDRESS_H

#include <sys/socket.h>

#include <netinet/in.h>
#include <linux/netlink.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

#include <Debug.h>

namespace ProtocolLearn {

class SocketAddress
{
public:
    SocketAddress();

    explicit SocketAddress(const sockaddr_in &sockaddr);
    explicit SocketAddress(const sockaddr_ll &sockaddr);
    explicit SocketAddress(const sockaddr_in6 &sockaddr);
    explicit SocketAddress(const sockaddr_nl &sockaddr);

    size_t getLength() const{ return d.socketaddrLength; }
    void setLength(size_t length);

    // Generic sockaddr function.
    template<class T>
    const T *getSockaddr() const{
        pl_assert(sizeof(T) <= sizeof(d.storage));

        return reinterpret_cast<const T*>(&d.storage);
    }

    template<class T>
    T *getSockaddr() {
        pl_assert(sizeof(T) <= sizeof(d.storage));

        return reinterpret_cast<const T*>(&d.storage);
    }

private:
    void setSocketaddr(const void *sockaddr, size_t length);

    struct Data{
        sockaddr_storage storage;
        size_t socketaddrLength;
    };

    Data d;
};

} // ProtocolLearn

#endif // SOCKETADDRESS_H
