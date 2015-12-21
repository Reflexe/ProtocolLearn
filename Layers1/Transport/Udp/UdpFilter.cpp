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

#include "UdpFilter.h"

namespace ProtocolLearn {
namespace Udp {

UdpFilter::UdpFilter()
{
}

UdpFilter::DropReasonType UdpFilter::checkByProtocol(const UdpPacket &filteredPacket) {
    if(mIsInvalidChecksumCritical == true && filteredPacket.isValidChecksum() == false)
        return InvalidChecksum;

    return DropReason::None;
}

UdpFilter::DropReasonType UdpFilter::checkByPreviousPacket(const UdpPacket &filteredPacket) {
    if(mIsIncorrectSourcePortCritical == true && filteredPacket.getSourcePort() != mUdpSession.destinationPort)
        return InvalidSourcePort;
    if(filteredPacket.getDestinationPort() != mUdpSession.sourcePort)
        return InvalidDestinationPort;

    return DropReason::None;
}

void UdpFilter::filterByPacket(const UdpPacket &filteringPacket) {
    mUdpSession.destinationPort = filteringPacket.getDestinationPort();
    mUdpSession.sourcePort = filteringPacket.getSourcePort();
}

bool UdpFilter::isIncorrectSourcePortCritical() const
{
    return mIsIncorrectSourcePortCritical;
}

void UdpFilter::setIsIncorrectSourcePortCritical(bool isIncorrectSourcePortCritical)
{
    mIsIncorrectSourcePortCritical = isIncorrectSourcePortCritical;
}

bool UdpFilter::isInvalidChecksumCritical() const
{
    return mIsInvalidChecksumCritical;
}

void UdpFilter::setIsInvalidChecksumCritical(bool isInvalidChecksumCritical)
{
    mIsInvalidChecksumCritical = isInvalidChecksumCritical;
}

} // namespace Udp
} // namespace ProtocolLearn
