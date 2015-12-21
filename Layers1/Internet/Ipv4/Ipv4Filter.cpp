/**
 * @file Ipv4Filter.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
  Ipv4Filter
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) %YEAR% shrek0
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

#include "Ipv4Filter.h"

namespace ProtocolLearn {
namespace Ipv4 {

Ipv4Filter::Ipv4Filter()
{
}

Ipv4Filter::DropReasonType Ipv4Filter::checkByProtocol(const Ipv4Packet &filteredPacket) {
    if(filteredPacket.getVersion() != 4)
        return InvalidVersion;
    if(filteredPacket.isValidPacket() == false)
        return InvalidPacket;
    if(filteredPacket.getParser().isValidOptions() == false)
        return InvalidOptions;
    if(filteredPacket.getTotalLength() != filteredPacket.getPacketLength())
        return InvalidTotalLength;
    if(filteredPacket.getTimeToLive() == 0)
        return InvalidTimeToLive;
    if(filteredPacket.isChecksumValid() == false)
        return InvalidChecksum;

    return DropReason::None;
}

Ipv4Filter::DropReasonType Ipv4Filter::checkByPreviousPacket(const Ipv4Packet &filteredPacket) {
    if(filteredPacket.getDestination() != ourSession.sourceAddress)
        return InvalidDestinationAddres;
    if(filteredPacket.getSource() != ourSession.destinationAddress)
        return InvalidSourceAddress;
    if(filteredPacket.getProtocol() != ourSession.protocol)
        return InvalidProtocol;

    return DropReason::None;
}

void Ipv4Filter::filterByPacket(const Ipv4Packet &filteringPacket) {
    ourSession.sourceAddress = filteringPacket.getSource();
    ourSession.destinationAddress = filteringPacket.getDestination();

    ourSession.protocol = filteringPacket.getProtocol();
}

} // ProtocolLearn
} // Ipv4
