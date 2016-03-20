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

#include "Ipv4Fragmenter.h"

#include <limits>

namespace ProtocolLearn {
namespace Ipv4 {

Ipv4Fragmenter::Ipv4Fragmenter()
{
}

std::list<Ipv4Packet> Ipv4Fragmenter::fragmentPacket(const Ipv4Packet &packetToSplit, OctetVector::SizeType maximumPacketSize) {
    pl_assert(maximumPacketSize > Ipv4Packet::MinimumHeaderLength);
    pl_assert(packetToSplit.getPacketLength() <= std::numeric_limits<uint16_t>::max());
    pl_assert(maximumPacketSize <= std::numeric_limits<uint16_t>::max());

    if(packetToSplit.getPacketLength() <= maximumPacketSize)
        return std::list<Ipv4Packet>{packetToSplit};

    // Get only the header.
    Ipv4Packet temporaryPacket;
    temporaryPacket.fromRawPacket(OctetVector{packetToSplit.getVectorHeader()});
    clearNotRequiredOptions(temporaryPacket);

    pl_assert(maximumPacketSize > temporaryPacket.getHeaderLength());

    // Align the data size to 8.
    OctetVector::SizeType maximumDataSize = maximumPacketSize-temporaryPacket.getHeaderLength();
    if(maximumDataSize % 8 != 0)
        maximumDataSize -= (maximumDataSize % 8);

    auto dataFragmentslist = packetToSplit.getVectorData().splitTo(maximumDataSize);
    std::list<Ipv4Packet> packetFragmentsList;

    OctetVector::SizeType fragmentOffsetBytes = 0;
    temporaryPacket.setMoreFragments(true);

    for(auto &dataFragment : dataFragmentslist) {
        temporaryPacket.importData(std::move(dataFragment));
        temporaryPacket.setFragmentOffset(static_cast<uint16_t>(fragmentOffsetBytes/8));

        packetFragmentsList.push_back(temporaryPacket);

        fragmentOffsetBytes += maximumDataSize;
    }

    packetFragmentsList.back().setMoreFragments(false);

    return packetFragmentsList;
}

void Ipv4Fragmenter::clearNotRequiredOptions(Ipv4Packet &ipv4Packet) {
    // Remove the option its copied flag is false.
    for(const auto &option : ipv4Packet.getParser().getMap()) {
        if(option.second.optionID.copiedFlag == false)
            ipv4Packet.getParser().removeOption(option.first);
    }
}

} // namespace Ipv4
} // namespace ProtocolLearn

