/**
 * @file arpethernetipv4filter.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
  ArpEthernetIpv4Filter
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

#include "ArpEthernetIpv4Filter.h"

namespace ProtocolLearn {
namespace Arpv4 {

ArpEthernetIpv4Filter::ArpEthernetIpv4Filter()
{
}

void ArpEthernetIpv4Filter::filterByPacket(const ArpEthernetIpv4Packet &filteringPacket) {
    mSession.senderIpv4Address = filteringPacket.getSenderIpv4Address();
    mSession.senderMacAddress = filteringPacket.getSenderMacAddress();

    mSession.targetIpv4Address = filteringPacket.getTargetIpv4Address();
    mSession.targetMacAddress = filteringPacket.getTargetMacAddress();
}

ArpEthernetIpv4Filter::DropReasonType ArpEthernetIpv4Filter::checkByPreviousPacket(const ArpEthernetIpv4Packet &filteredPacket) {
    if(filteredPacket.getSenderIpv4Address() != mSession.targetIpv4Address)
        return InvalidSenderIpv4Address;
    if(filteredPacket.getTargetMacAddress() != mSession.senderMacAddress)
        return InvalidTargetMacAddress;
    if(filteredPacket.getTargetIpv4Address() != mSession.senderIpv4Address)
        return InvalidTragetIpv4Address;

    return DropReason::None;
}

} // Arpv4
} // ProtocolLearn
