/**
 * @file PacketStreamUnderDataStream.h
 * @author shrek0 (shrek0.tk@gmail.com)
  PacketStreamUnderDataStream
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

#ifndef PACKETSTREAMUNDERDATASTREAM_H
#define PACKETSTREAMUNDERDATASTREAM_H

#include "PacketStream.h"
#include "DataStream.h"

namespace ProtocolLearn {

template<typename PacketType, typename FilterType, typename DataStreamType=DataStream>
class PacketStreamUnderDataStream : public PacketStream<PacketType, FilterType>
{
public:
    typedef PacketStream<PacketType, FilterType> PacketStreamType;

    explicit PacketStreamUnderDataStream(DataStreamType &dataStream)
        : mDataStream(dataStream)
    {
        mDataStream.setMinimumReceiveDataSize(PacketType::MinimumHeaderLength);
    }

    virtual void _recv(PacketType &packet, const Timeout &timeout) override{
        auto rawPacket = mDataStream.receiveData(timeout);

        // Import data only if the data if at least the minmum header length.
        pl_assert(rawPacket.size() >= PacketType::MinimumHeaderLength);

        packet.fromRawPacket(std::move(rawPacket));
    }

    virtual void _send(PacketType &packet) override
    {
        mDataStream.sendData(packet.toVectorRawPacket());
    }

    virtual void setMinimumDataLength(OctetVector::SizeType minimumDataLength) override{
        if(mDataStream.getMinimumReceiveDataSize() == minimumDataLength)
            return;

        PacketStream<PacketType, FilterType>::setMinimumDataLength(minimumDataLength);
        mDataStream.setMinimumReceiveDataSize(minimumDataLength);
    }

    DataStreamType &getDataStream() { return mDataStream; }
    const DataStreamType &getDataStream() const{ return mDataStream; }

private:
    DataStreamType &mDataStream;
};

} // ProtocolLearn

#endif // PACKETSTREAMUNDERDATASTREAM_H
