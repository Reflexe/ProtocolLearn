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

    if(isError(filteredPacket.getType())) {
        if(filteredPacket.getDataLength() < Ipv4::Ipv4Packet::MinimumHeaderLength)
            return InvalidOriginalDatagramSize;
    }

    return DropReason::None;
}

IcmpFilter::DropReasonType IcmpFilter::checkByPreviousPacket(const IcmpPacket &filteredPacket) {
    auto icmpType = filteredPacket.getType();
    if (isErrorsAccepted() && isError(icmpType))
        return WrongType;

    if (isError(icmpType)) {
        if (!isErrorsAccepted())
            return WrongType;

        auto originalDatagram = filteredPacket.getOriginalDatagram();
        const auto &ipv4Session = mIcmpSession.mIpv4Session;

        if (originalDatagram.getIdentification() != ipv4Session.identification
                || originalDatagram.getDestination().toInt() != ipv4Session.destinationAddress
                || originalDatagram.getSource().toInt() != ipv4Session.sourceAddress
                || originalDatagram.getProtocol() != ipv4Session.protocol)
            return UnknownResponsePacket;

        if(mIcmpSession.mIpv4DataSize > originalDatagram.getDataLength())
            return UnknownResponsePacket;

        if(std::equal(mIcmpSession.mIpv4Data.begin(),
                      mIcmpSession.mIpv4Data.begin() + mIcmpSession.mIpv4DataSize,
                      filteredPacket.getVectorData().begin()) == false)
            return UnknownResponsePacket;
    }

    if(filteredPacket.getSequence() != mIcmpSession.sequence)
        return InvalidSequence;
    if(filteredPacket.getId() != mIcmpSession.id)
        return InvalidID;

    if(mIcmpSession.type == ICMP_ECHO && icmpType != ICMP_ECHOREPLY)
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

void IcmpFilter::setIsErrorsAccepted(bool isErrorsAccepted,
                                     const Ipv4::Ipv4Packet &ipv4Packet,
                                     const OctetVector &data) {
    mIsErrorsAccepted = isErrorsAccepted;

    if(isErrorsAccepted == false)
        return;

    mIcmpSession.mIpv4Session.destinationAddress = ipv4Packet.getDestination().toInt();
    mIcmpSession.mIpv4Session.sourceAddress = ipv4Packet.getSource().toInt();
    mIcmpSession.mIpv4Session.protocol = ipv4Packet.getProtocol();

    // The packet is synced, there's no need to call toVectorRawPacket.
    // The required data is only eight bytes.
    auto endIterator = data.size() < 8 ? data.cend() : data.cbegin() + 8;
    mIcmpSession.mIpv4DataSize = data.size() < 8 ? data.size() : 8;

    std::copy(data.begin(), endIterator, mIcmpSession.mIpv4Data.begin());
}

void IcmpFilter::filterByPacket(const IcmpPacket &filteringPacket) {
    mIcmpSession.type = filteringPacket.getType();
    mIcmpSession.code = filteringPacket.getCode();
    mIcmpSession.sequence = filteringPacket.getSequence();
    mIcmpSession.id = filteringPacket.getId();
}

} // ProtocolLearn
} // Icmp
