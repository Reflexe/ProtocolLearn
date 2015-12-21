/**
 * @file icmppacket.cpp
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

#include "IcmpPacket.h"

#include "NetworkFunctions.h"

namespace ProtocolLearn {
namespace Icmp {

IcmpPacket::IcmpPacket()
{
}

Ipv4::Ipv4Packet IcmpPacket::getOriginalDatagram() const{
    Ipv4::Ipv4Packet ipv4Packet;
    ipv4Packet.fromRawPacket(getVectorData());
    return ipv4Packet;
}

bool IcmpPacket::isChecksumValid() const{
    auto icmpHeaderVector = getVectorHeader();

    icmpHeaderVector.getAsObject<IcmpHeader>().checksum = 0;

    return calculateChecksum(icmpHeaderVector, getVectorData()) == getChecksum();
}

void IcmpPacket::onPacketExport()
{
    updateChecksum();
}

uint16_t IcmpPacket::calculateChecksum(const OctetVector &header, const OctetVector &data)
{
    return NetworkFunctions::calculateInternetChecksum(NetworkFunctions::VectorsList{header, data});
}

void IcmpPacket::updateChecksum() {
    getHeader().checksum = 0;
    setChecksum(calculateChecksum(getVectorHeader(), getVectorData()));
}

} // ProtocolLearn
} // Icmp
