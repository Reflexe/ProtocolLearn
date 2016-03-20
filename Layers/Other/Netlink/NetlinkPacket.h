/**
 * @file netlinkPacket.h
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

#ifndef NETLINKPACKET_H
#define NETLINKPACKET_H

#include "PacketWrapper.h"

#include <linux/netlink.h>

namespace ProtocolLearn {
namespace Netlink {

class NetlinkPacket : public PacketWrapper<nlmsghdr>
{
public:
    NetlinkPacket();

    uint32_t getLength() const{ return getHeader().nlmsg_len; }
    uint16_t getType() const{ return getHeader().nlmsg_type; }
    uint32_t getFlags() const{ return getHeader().nlmsg_seq; }
    uint32_t getSequence() const{ return getHeader().nlmsg_seq; }
    uint32_t getProccessID() const{ return getHeader().nlmsg_pid; }

    void setType(uint16_t type) { getHeader().nlmsg_type = type; }
    void setFlags(uint16_t flags) { getHeader().nlmsg_flags = flags; }
    void setSequence(uint32_t sequence) { getHeader().nlmsg_seq = sequence; }
    void setProccessID(uint32_t proccessID) { getHeader().nlmsg_pid = proccessID; }

private:
    virtual void onPacketExport() override final;
};

} // Netlink
} // ProtocolLearn

#endif // NETLINKPACKET_H
