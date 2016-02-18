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

#include "PacketCache.h"

namespace ProtocolLearn {

ProtocolLearn::PacketCache::PacketCache(PTime::Seconds seconds, PTime::MicroSeconds microSeconds)
    : timeVal{seconds, microSeconds}
{
}

void PacketCache::updateCache() {
    for(const auto &packet : packets) {        
        if(packet.second.timeout.check())
            packets.erase(packet.first);
    }
}

void PacketCache::deletePacket(const PacketCache::PacketID &packetID)
{
    packets.erase(packetID);
}

PacketCache::PacketID PacketCache::addPacket(const OctetVector &data) {
    // Find an empty place.
    while(packets.count(++lastPacketID) != 0)
    {
        updateCache();
    }

    packets[lastPacketID] = SavedPacketType{data, Timeout(timeVal, true)};

    return lastPacketID;
}

void PacketCache::getPacket(PacketCache::PacketID packetID, OctetVector &data) const
{
    data = packets.at(packetID).dataType;
}

PacketCache::PacketID PacketCache::getLastPacketID()
{
    return lastPacketID;
}

}
