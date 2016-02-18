/**
 * @file socketaddress.cpp
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

#include "SocketAddress.h"

#include <cstring>

namespace ProtocolLearn {

SocketAddress::SocketAddress() {
    ::memset(&d.storage, 0, sizeof(d.storage));
    d.socketaddrLength = 0;
}

SocketAddress::SocketAddress(const sockaddr_in &sockaddr) : SocketAddress()
{
    setSocketaddr(reinterpret_cast<const void *>(&sockaddr), sizeof(sockaddr));
}

SocketAddress::SocketAddress(const sockaddr_ll &sockaddr) : SocketAddress()
{
    setSocketaddr(reinterpret_cast<const void *>(&sockaddr), sizeof(sockaddr));
}

SocketAddress::SocketAddress(const sockaddr_in6 &sockaddr) : SocketAddress()
{
    setSocketaddr(reinterpret_cast<const void *>(&sockaddr), sizeof(sockaddr));
}

SocketAddress::SocketAddress(const sockaddr_nl &sockaddr) : SocketAddress()
{
    setSocketaddr(reinterpret_cast<const void *>(&sockaddr), sizeof(sockaddr));
}

void SocketAddress::setLength(size_t length) {
    // storage must contain at least the length size.
    pl_assert(length <= sizeof(d.storage));

    d.socketaddrLength = length;
}

void SocketAddress::setSocketaddr(const void *sockaddr, size_t length) {
    setLength(length);

    ::memset(&d.storage, 0, length);
    ::memmove(&d.storage, sockaddr, length);
}

} // ProtocolLearn
