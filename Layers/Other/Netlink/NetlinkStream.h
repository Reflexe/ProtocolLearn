/**
 * @file netlinkStream.h
 * @author shrek0 (shrek0.tk@gmail.com)
  NetlinkStream
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

#ifndef NETLINKSTREAM_H
#define NETLINKSTREAM_H

#include "PacketStreamUnderDataStream.h"

#include "NetlinkPacket.h"
#include "NetlinkFilter.h"

namespace ProtocolLearn {
namespace Netlink {

typedef PacketStreamUnderDataStream<NetlinkPacket, NetlinkFilter> NetlinkStream;

} // Netlink
} // ProtocolLearn

#endif // NETLINKSTREAM_H
