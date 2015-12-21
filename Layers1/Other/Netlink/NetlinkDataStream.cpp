/**
 * @file netlinkdatastream.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
  NetlinkDataStream
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

#include "NetlinkDataStream.h"

namespace ProtocolLearn {
namespace Netlink {

NetlinkDataStream::NetlinkDataStream(NetlinkStream &netlinkStream, uint16_t type, uint32_t proccessID, uint16_t flags)
    : DataStreamUnderPacketStream(netlinkStream) {

    mSendPacket.setType(type);
    mSendPacket.setProccessID(proccessID);
    mSendPacket.setFlags(flags);

    getPacketStream().getFilter().filterByPacket(mSendPacket);
}

} // ProtocolLearn
} // Netlink
