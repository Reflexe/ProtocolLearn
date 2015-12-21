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

#include "IcmpFilter.h"

#include "Ipv4Packet.h"

namespace ProtocolLearn {
namespace Icmp {

IcmpFilter::IcmpFilter()
{
}

IcmpFilter::DropReasonType IcmpFilter::checkByProtocol(const IcmpPacket &filteredPacket) {
    if(filteredPacket.isChecksumValid() == false)
        return InvalidChecksum;

    if((filteredPacket.getType() == ICMP_TIME_EXCEEDED || filteredPacket.getType() == ICMP_DEST_UNREACH)
            && filteredPacket.getDataLength() <= (Ipv4::Ipv4Packet::MinimumHeaderLength+64))
        return NoPlaceForOriginalDatagram;

    return DropReason::None;
}

IcmpFilter::DropReasonType IcmpFilter::checkByPreviousPacket(const IcmpPacket &filteredPacket) {
    auto icmpType = filteredPacket.getType();
    if(isErrorsAccepted() && (icmpType == ICMP_TIME_EXCEEDED || icmpType == ICMP_DEST_UNREACH))
        return WrongType;

    if(filteredPacket.getSequence() != icmpSession.sequence)
        return InvalidSequence;
    if(filteredPacket.getId() != icmpSession.id)
        return InvalidID;

    if(icmpSession.type == ICMP_ECHO && filteredPacket.getType() != ICMP_ECHOREPLY)
        return WrongType;

    return DropReason::None;
}

bool IcmpFilter::isErrorsAccepted() const
{
    return mIsErrorsAccepted;
}

void IcmpFilter::setIsErrorsAccepted(bool isErrorsAccepted)
{
    mIsErrorsAccepted = isErrorsAccepted;
}

void IcmpFilter::filterByPacket(const IcmpPacket &filteringPacket) {
    icmpSession.type = filteringPacket.getType();
    icmpSession.code = filteringPacket.getCode();
    icmpSession.sequence = filteringPacket.getSequence();
    icmpSession.id = filteringPacket.getId();
}

} // ProtocolLearn
} // Icmp
