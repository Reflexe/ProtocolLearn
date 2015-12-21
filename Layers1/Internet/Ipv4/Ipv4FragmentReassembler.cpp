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

#include "Ipv4FragmentReassembler.h"

namespace ProtocolLearn {
namespace Ipv4 {

Ipv4FragmentReassembler::Ipv4FragmentReassembler()
{
}

Ipv4FragmentReassembler::Ipv4FragmentReassembler(const Ipv4Packet &firstPacket)
    : mFirstPacket(firstPacket) {

    pl_assert(firstPacket.getFragmentOffset() == 0);
    mOffsetsAndDatas[0] = std::move(mFirstPacket.getVectorData());

    mFirstPacket.removeData();
}

void Ipv4FragmentReassembler::reset(const Ipv4Packet &firstPacket) {    
    pl_assert(firstPacket.getFragmentOffset() == 0);

    mLastOffset = 0;
    mTotalLength = 0;

    mFirstPacket = firstPacket;

    mOffsetsAndDatas[0] = std::move(mFirstPacket.getVectorData());

    mFirstPacket.removeData();
}

bool Ipv4FragmentReassembler::isPacketRequired(const Ipv4Packet &ipv4Packet) const{
    return mFirstPacket.getDestination() == ipv4Packet.getDestination()
            && mFirstPacket.getSource() == ipv4Packet.getSource()
            && mFirstPacket.getIdentification() == ipv4Packet.getIdentification()
            && mFirstPacket.getProtocol() == ipv4Packet.getProtocol()
            && (ipv4Packet.getDataLength() % 8 == 0
                || (ipv4Packet.getMoreFragmentsFlag() == false && mLastOffset == 0)) /* The last fragment doesn't must be 8 byte aligned */
            && ipv4Packet.getFragmentOffset() != 0 /* We already received the first fragment */
            && mOffsetsAndDatas.count(ipv4Packet.getFragmentOffset()) == 0;
//            && isValidOffset(ipv4Packet.getFragmentOffset(), ipv4Packet.getDataLength());
}

void Ipv4FragmentReassembler::insertPacket(const Ipv4Packet &ipv4Packet) {
    const auto &offset = ipv4Packet.getFragmentOffset();

    // You must call isPacketRequired before.
    pl_assert(mOffsetsAndDatas.count(offset) == 0);

    mOffsetsAndDatas[offset] = ipv4Packet.getVectorData();

    if(ipv4Packet.getMoreFragmentsFlag() == false) {
        pl_crap("last fragment found");

        pl_assert(mLastOffset == 0);
        mLastOffset = offset;
        mTotalLength = (mLastOffset*8) + ipv4Packet.getDataLength();
    }
}

bool Ipv4FragmentReassembler::isAllOffsetsFilled() {
    // If we don't get the last packet.
    if(mLastOffset == 0)
        return false;

    OctetVector::SizeType nextRequiredOffset = 0;

    // std::map is ordered. Therefore, we'll get the lower offsets first.
    for(const auto &offsetAndData : mOffsetsAndDatas) {
        if(nextRequiredOffset == mLastOffset) {
            pl_crap("The packet is ready!");

            return true;
        }

        if(offsetAndData.first != nextRequiredOffset) {
            pl_crap("An hole in the fragment offsets");

            return false;
        }

        pl_assert(offsetAndData.second.size() % 8 == 0);
        nextRequiredOffset += offsetAndData.second.size()/8;
    }

    pl_crap("There're no more fragments");

    // We've not found the next offset and there's no more insered packets.
    return false;
}

Ipv4Packet Ipv4FragmentReassembler::toPacket() const{
    OctetVector data;

    for(const auto &offsetAndData : mOffsetsAndDatas)
    {
        data.add(offsetAndData.second);
    }

    Ipv4Packet ipv4Packet = mFirstPacket;
    ipv4Packet.importData(data);

    pl_assert(ipv4Packet.isValidPacket() == true);

    ipv4Packet.setMoreFragments(false);

    return ipv4Packet;
}

bool Ipv4FragmentReassembler::isFirstFragment(const Ipv4Packet &ipv4Packet)
{
    return ipv4Packet.getFragmentOffset() == 0 && ipv4Packet.getMoreFragmentsFlag() == true;
}

bool Ipv4FragmentReassembler::isFragment(const Ipv4Packet &ipv4Packet)
{
    return ipv4Packet.getFragmentOffset() != 0 || ipv4Packet.getMoreFragmentsFlag() == true;
}

//bool Ipv4FragmentReassembler::isValidOffset(uint16_t offset, OctetVector::SizeType dataSize)
//{   // There's no integer overflow here, in the badest option (offset set to 65535), offset*8 will be 524280 (not even close to 2**32-1).
//    return ((offset*8u) + dataSize) < std::numeric_limits<uint16_t>::max();
//}

} // namespace Ipv4
} // namespace ProtocolLearn

