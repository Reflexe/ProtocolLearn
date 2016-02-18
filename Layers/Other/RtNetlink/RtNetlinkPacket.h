/**
 * @file rtnetlinkPacket.h
 * @author shrek0 (shrek0.tk@gmail.com)
  RtNetlinkPacket
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

#ifndef RTNETLINKPACKET_H
#define RTNETLINKPACKET_H

#include "RtAttrParser.h"

#include "PacketWrapper.h"

namespace ProtocolLearn {
namespace RtNetlink {

class RtNetlinkPacket : public PacketWrapper<rtmsg>
{
public:
    RtNetlinkPacket();

    uint8_t getFamily() const{ return getHeader().rtm_family; }
    uint8_t getDestinationLength() const{ return getHeader().rtm_dst_len; }
    uint8_t getSourceLength() const{ return getHeader().rtm_src_len; }
    uint8_t getTos() const{ return getHeader().rtm_tos; }
    uint8_t getTableId() const{ return getHeader().rtm_table; }
    uint8_t getProtocol() const{ return getHeader().rtm_protocol; }
    uint8_t getScope() const{ return getHeader().rtm_scope; }
    uint8_t getType() const{ return getHeader().rtm_type; }
    uint32_t getFlags() const{ return getHeader().rtm_flags; }

    void setFamily(uint8_t family) { getHeader().rtm_family = family; }
    void setDestinationLength(uint8_t destinationLength) { getHeader().rtm_dst_len = destinationLength; }
    void setSourceLength(uint8_t sourceLength) { getHeader().rtm_src_len = sourceLength; }
    void setTos(uint8_t tos) { getHeader().rtm_tos = tos; }
    void setTableId(uint8_t tableId) { getHeader().rtm_table = tableId; }
    void setProtocol(uint8_t protocol) { getHeader().rtm_protocol = protocol; }
    void setScope(uint8_t scope) { getHeader().rtm_scope = scope; }
    void setType(uint8_t type) { getHeader().rtm_type = type; }
    void setFlags(uint32_t flags) { getHeader().rtm_flags = flags; }

    const RtattrParser &getParser() const{ return rtattrParser; }
    RtattrParser &getParser() { return rtattrParser; }

protected:
    virtual void onPacketImport() override final;
    virtual void onPacketExport() override final;

private:
    RtattrParser rtattrParser;

};

} // ProtocolLearn
} // RtNetlink

#endif // RTNETLINKPACKET_H
