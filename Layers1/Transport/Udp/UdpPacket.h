/**
 * @file
 * @author shrek0 (shrek0.tk@gmail.com)
 
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

#ifndef PROTOCOLLEARN_UDPPACKET_H
#define PROTOCOLLEARN_UDPPACKET_H

#include <cstdint>

#include "PacketWrapper.h"
#include "ByteOrder.h"

namespace ProtocolLearn {
namespace Udp {

struct UdpHeader{
    uint16_t sourcePort;
    uint16_t destinationPort;
    uint16_t length;
    uint16_t checksum;
}__attribute__((packed));

class UdpPacket : public PacketWrapper<UdpHeader>
{
public:
    UdpPacket();

    uint16_t getSourcePort() const{ return ByteOrder::networkToHost(getHeader().sourcePort); }
    uint16_t getDestinationPort() const{ return ByteOrder::networkToHost(getHeader().destinationPort); }
    uint16_t getLength() const{ return ByteOrder::networkToHost(getHeader().length); }
    uint16_t getChecksum() const{ return ByteOrder::networkToHost(getHeader().checksum); }

    void setSourcePort(uint16_t sourcePort) { getHeader().sourcePort = ByteOrder::hostToNetwork(sourcePort); }
    void setDestinationPort(uint16_t destinationPort) { getHeader().destinationPort = ByteOrder::hostToNetwork(destinationPort); }
    void setLength(uint16_t length) { getHeader().length = ByteOrder::hostToNetwork(length); }
    void setChecksum(uint16_t checksum) { getHeader().checksum = ByteOrder::hostToNetwork(checksum); }

    bool isValidChecksum() const;

    OctetVector &getpseudoHeader() { return mPseudoHeader; }

private:
    void updateChecksum();
    virtual void onPacketExport() override final;

    static uint16_t calculateChecksum(const OctetVector &pseudoHeader, const OctetVector &header, const OctetVector &data);

    OctetVector mPseudoHeader;
};

} // namespace Udp
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_UDPPACKET_H
