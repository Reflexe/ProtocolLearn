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

#include "MultiStreamDataStream.h"

#include <limits>

namespace ProtocolLearn {

MultiStreamDataStream::MultiStreamDataStream(DataStream &dataStream)
    : DataStreamUnderDataStream(dataStream), packetCache(1, 0)
{
}

void MultiStreamDataStream::receiveData(OctetVector &data, StreamID streamID) {
    updateCache();

    auto streamLastPacket = getLastPacketIDByStream(streamID);
    auto lastPacketID = packetCache.getLastPacketID();

    // Stream's last packetID cannot be grather than the lastPacketID.
    pl_assert(streamLastPacket <= lastPacketID);

    if(lastPacketID == streamLastPacket) { // We already received all the cached packets.
        derivedStreams[streamID] = receiveNewPacket(data);
    } else {
        packetCache.getPacket(streamLastPacket, data);
        derivedStreams[streamID] += ++streamLastPacket;
    }
}

void MultiStreamDataStream::setLastPacketByStream(const PacketCache::PacketID &packetID, const MultiStreamDataStream::StreamID &streamID)
{
    derivedStreams[packetID] = streamID;
}

MultiStreamDataStream::StreamID MultiStreamDataStream::registerNewStream() {
    if(lastStreamID == std::numeric_limits<StreamID>::max())
        pl_assert(false);

    StreamID streamID = lastStreamID+=1;

    derivedStreams[streamID] = 0;

    return streamID;
}

void MultiStreamDataStream::unregisterStream(const MultiStreamDataStream::StreamID &streamID)
{
    derivedStreams.erase(streamID);
}

void MultiStreamDataStream::updateCache() {
    packetCache.updateCache();

    // If the cache were cleared.
    if(packetCache.getLastPacketID() == 0) {

        // Set all streams to zero.
        for(const auto &stream : derivedStreams)
        {
            derivedStreams[stream.first] = 0;
        }
    }
}

PacketCache::PacketID MultiStreamDataStream::getLastPacketIDByStream(const MultiStreamDataStream::StreamID &streamID) const
{
    return derivedStreams.at(streamID);
}

PacketCache::PacketID MultiStreamDataStream::receiveNewPacket(OctetVector &data) {
    DataStreamUnderDataStream::receiveData(data);

    return packetCache.addPacket(data);
}

}
