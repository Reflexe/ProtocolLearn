/**
 * @file icmppacket.h
 * @author shrek0 (shrek0.tk@gmail.com)
  IcmpPacket
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

#ifndef ICMPPACKET_H
#define ICMPPACKET_H

#include <linux/icmp.h>

#include "ByteOrder.h"
#include "PacketWrapper.h"
#include "../Ipv4Packet.h"

namespace ProtocolLearn {
namespace Icmp {

struct IcmpHeader{
    uint8_t type ;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
}__attribute__((packed));

class IcmpPacket : public PacketWrapper<IcmpHeader>
{
public:
    IcmpPacket();

    uint8_t getType() const{ return getHeader().type; }
    uint8_t getCode() const{ return getHeader().code; }
    uint16_t getChecksum() const{ return ByteOrder::networkToHost(getHeader().checksum); }
    uint16_t getId() const{ return ByteOrder::networkToHost(getHeader().id); }
    uint16_t getSequence() const{ return ByteOrder::networkToHost(getHeader().sequence); }

    void setType(uint8_t type) { getHeader().type = type; }
    void setCode(uint8_t code) { getHeader().code = code; }
    void setChecksum(uint16_t checksum) { getHeader().checksum = ByteOrder::hostToNetwork(checksum); }
    void setId(uint16_t id) { getHeader().id = ByteOrder::hostToNetwork(id); }
    void setSequence(uint16_t sequence) { getHeader().sequence = ByteOrder::hostToNetwork(sequence); }

    Ipv4::Ipv4Packet getOriginalDatagram() const;

    bool isChecksumValid() const;

protected:
    virtual void onPacketExport() override final;

    static uint16_t calculateChecksum(const OctetVector &header, const OctetVector &data);

    void updateChecksum();
};

} // Icmp
} // ProtocolLearn

#endif // ICMPPACKET_H
