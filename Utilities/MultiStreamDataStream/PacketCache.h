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

#ifndef PACKETCACHE_H
#define PACKETCACHE_H

#include <unordered_map>

#include "Timeout.h"
#include "OctetVector.h"

namespace ProtocolLearn {

class PacketCache
{
public:
    typedef uint32_t PacketID;

    PacketCache(PTime::Seconds seconds, PTime::MicroSeconds microSeconds);

    /**
     * @brief updateCache Clear the cache if the cache's timeout passed.
     */
    void updateCache();

    /**
     * @brief deletePacket Delete a packet from the cache.
     * @param packetID
     */
    void deletePacket(const PacketID &packetID);

    /**
     * @brief addPacket  Add packet to cache.
     * @param data The packet.
     * @return the new packet's ID.
     */
    PacketID addPacket(const OctetVector &data);

    /**
     * @brief getPacket  Fill a packet from the cache.
     * @param packetID
     * @param data The packet to fill.
     */
    void getPacket(PacketID packetID, OctetVector &data) const;

    PacketID getLastPacketID();

private:
    Timeout::TimeVal timeVal;

    PacketID lastPacketID = 0;

    struct SavedPacketType{
        /**
         * @brief dataType  The saved data.
         */
        OctetVector dataType;

        /**
         * @brief timeout  The time this saved packet will expire.
         */
        Timeout timeout;
    };

    /**
     * @brief packets
     */
    std::unordered_map<PacketID, SavedPacketType> packets;
};

}

#endif // PACKETCACHE_H
