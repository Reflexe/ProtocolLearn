/**
 * @file netlinkpacket.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
  NetlinkPacket
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

#include "NetlinkPacket.h"

#include <limits>

namespace ProtocolLearn {
namespace Netlink {

NetlinkPacket::NetlinkPacket()
{
}

void NetlinkPacket::onPacketExport() {
    pl_assert(getPacketLength() < std::numeric_limits<uint32_t>::max());

    getHeader().nlmsg_len = static_cast<uint32_t>(getPacketLength());
}

} // ProtocolLearn
} // Netlink
