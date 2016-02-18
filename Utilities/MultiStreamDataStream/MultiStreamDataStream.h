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

#ifndef MULTISTREAMDATASTREAM_H
#define MULTISTREAMDATASTREAM_H

#include "DataStreamUnderDataStream.h"

#include <map>

#include "PacketCache.h"

namespace ProtocolLearn {

/**
 * @brief The MultiStreamDataStream class  Save the received packets and support multi streams.
 */

class MultiStreamDataStream : public DataStreamUnderDataStream
{
public:
    typedef uint16_t StreamID;

    /**
     * @brief MultiStreamDataStream
     * @param dataStream  the original data stream.
     */
    explicit MultiStreamDataStream(DataStream &dataStream);

    void receiveData(OctetVector &data, StreamID streamID);

    void setLastPacketByStream(const PacketCache::PacketID &packetID, const StreamID &streamID);
    PacketCache::PacketID getLastPacketIDByStream(const StreamID &streamID) const;

    StreamID registerNewStream();
    void unregisterStream(const StreamID &streamID);

    PacketCache &getPacketCache() { return packetCache; }

private:
    void updateCache();

    PacketCache::PacketID receiveNewPacket(OctetVector &data);

    PacketCache packetCache;

    StreamID lastStreamID = 0;

    /**
     * @brief derivedStreams this variable desctibe the status of the divided streams.
     *
     * The saved inforormation is:
     * StreamID: Which stream is it.
     * PacketID: Where the streams is (how much packet the stream received and what the next PacketID it shoulds receive).
     */
    std::unordered_map<StreamID, PacketCache::PacketID> derivedStreams;
};

}

#endif // MULTISTREAMDATASTREAM_H
