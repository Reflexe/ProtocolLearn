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

#ifndef PROTOCOLLEARN_IPV4_IPV4FRAGMENTREASSEMBLER_H
#define PROTOCOLLEARN_IPV4_IPV4FRAGMENTREASSEMBLER_H

#include <map>

#include "Ipv4Packet.h"

namespace ProtocolLearn {
namespace Ipv4 {

class Ipv4FragmentReassembler
{
public:
    Ipv4FragmentReassembler();
    Ipv4FragmentReassembler(const Ipv4Packet &firstPacket);

    void reset(const Ipv4Packet &firstPacket);

    bool isPacketRequired(const Ipv4Packet &ipv4Packet) const;

    void insertPacket(const Ipv4Packet &ipv4Packet);

    bool isAllOffsetsFilled();

    Ipv4Packet toPacket() const;

    /**
     * @brief isFirstFragment  Returns true if the packet is a first fragment.
     * @param ipv4Packet
     * @return
     */
    static bool isFirstFragment(const Ipv4Packet &ipv4Packet);

    /**
     * @brief isFragment  Returns true if the packet is a part of a fragmented packet (but not the first fragment).
     * @param ipv4Packet
     * @return
     */
    static bool isFragment(const Ipv4Packet &ipv4Packet);

private:
    static bool isValidOffset(uint16_t offset, OctetVector::SizeType dataSize);

    std::map<uint32_t, OctetVector> mOffsetsAndDatas;

    uint32_t mLastOffset = 0;
    uint16_t mTotalLength = 0;

    Ipv4Packet mFirstPacket;
};

} // namespace Ipv4
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_IPV4_IPV4FRAGMENTREASSEMBLER_H
