/**
 * @file DataStreamUnderPacketStream.h
 * @author shrek0 (shrek0.tk@gmail.com)
  DataStreamUnderPacketStream
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

#ifndef DATASTREAMUNDERPACKETSTREAM_H
#define DATASTREAMUNDERPACKETSTREAM_H

#include "PacketStream.h"
#include "DataStream.h"

namespace ProtocolLearn {

template<typename _PacketStreamType, typename DataStreamType=DataStream>
class DataStreamUnderPacketStream : public DataStreamType
{
public:
    typedef _PacketStreamType PacketStreamType;
    typedef typename PacketStreamType::PacketType PacketType;

    explicit DataStreamUnderPacketStream(PacketStreamType &packetStream)
        : mPacketStream(packetStream)
    {
    }

    DataStreamUnderPacketStream(DataStreamUnderPacketStream &&) = default;
    DataStreamUnderPacketStream &operator =(DataStreamUnderPacketStream &&) = delete;

    // If you want to split a DataStreamUnderPacketStream, use MultiStreamDataStream.
    DataStreamUnderPacketStream(const DataStreamUnderPacketStream &) = delete;
    DataStreamUnderPacketStream &operator =(const DataStreamUnderPacketStream &) = delete;

    virtual OctetVector _recv(const Timeout &timeout) override{
        mPacketStream.receivePacket(mReceivePacket, timeout);
        return std::move(mReceivePacket.getVectorData());
    }

    virtual void _send(OctetVector &&data) override{
        mSendPacket.importData(std::move(data));
        mPacketStream.sendPacket(mSendPacket);
        mSendPacket.removeData();
    }

    virtual void setMinimumReceiveDataSize(OctetVector::SizeType minimumDataSize) override{
        // Don't change the minimum data size if we already know it.
        if(DataStreamType::getMinimumReceiveDataSize() == minimumDataSize)
            return;

        DataStreamType::setMinimumReceiveDataSize(minimumDataSize);
        mPacketStream.setMinimumDataLength(minimumDataSize); // Set the protocol filter to get only the right size packets.
    }

    PacketStreamType &getPacketStream() { return mPacketStream; }
    const PacketStreamType &getPacketStream() const{ return mPacketStream; }

    const PacketType &getSendPacket() const{ return mSendPacket; }
    PacketType &getSendPacket() { return mSendPacket; }

    const PacketType &getReceivePacket() const{ return mReceivePacket; }
    PacketType &getReceivePacket() { return mReceivePacket; }
private:
    PacketStreamType &mPacketStream;

protected:
    PacketType mReceivePacket;
    PacketType mSendPacket;

};

}

#endif // DATASTREAMUNDERPACKETSTREAM_H
