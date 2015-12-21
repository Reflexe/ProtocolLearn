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

#include "UdpPacket.h"

#include "NetworkFunctions.h"

namespace ProtocolLearn {
namespace Udp {

UdpPacket::UdpPacket()
{
}

bool UdpPacket::isValidChecksum() const{
    auto vectorHeader = getVectorHeader();

    vectorHeader.getAsObject<UdpHeader>().checksum = 0;

    return getChecksum() == calculateChecksum(mPseudoHeader, vectorHeader, getVectorData());
}

void UdpPacket::updateChecksum() {
    if((getPacketLength() % 2) != 0) {
        getVectorData().alignTo(2);
    }

    getHeader().checksum = 0;
    setChecksum(calculateChecksum(mPseudoHeader, getVectorHeader(), getVectorData()));
}

void UdpPacket::onPacketExport() {
    setLength(getPacketLength());

    updateChecksum();
}

uint16_t UdpPacket::calculateChecksum(const OctetVector &pseudoHeader, const OctetVector &header, const OctetVector &data)
{
    return NetworkFunctions::calculateInternetChecksum(NetworkFunctions::VectorsList{header, data});
}

} // namespace Udp
} // namespace ProtocolLearn

