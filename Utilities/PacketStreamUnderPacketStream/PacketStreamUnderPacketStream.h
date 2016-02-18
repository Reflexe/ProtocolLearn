/**
 * @file %{FILENAME}
 * @author shrek0 (shrek0.tk@gmail.com)
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) %YEAR% shrek0
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

#ifndef PROTOCOLLEARN_PACKETSTREAMUNDERPACKETSTREAM_H
#define PROTOCOLLEARN_PACKETSTREAMUNDERPACKETSTREAM_H

#include "PacketStream.h"

namespace ProtocolLearn {

template<typename PacketStreamType, typename BasePacketStreamType=PacketStream<typename PacketStreamType::PacketType, typename PacketStreamType::FilterType>>
class PacketStreamUnderPacketStream : public BasePacketStreamType
{
public:
    explicit PacketStreamUnderPacketStream(PacketStreamType &packetStreamType)
        : mPacketStream{packetStreamType}
    {
        mPacketStream.setMinimumDataLength(BasePacketStreamType::PacketType::MinimumHeaderLength);
    }

    virtual void _recv(typename BasePacketStreamType::PacketType &packet) override
    {
        return mPacketStream.receivePacket(packet);
    }

    virtual void _send(typename BasePacketStreamType::PacketType &packet) override
    {
        return mPacketStream.sendPacket(packet);
    }

    virtual void setMinimumDataLength(OctetVector::SizeType minimumHeaderLength) override{
        BasePacketStreamType::setMinimumDataLength(minimumHeaderLength);
        mPacketStream.setMinimumDataLength(minimumHeaderLength);
    }

    PacketStreamType &getPacketStream() { return mPacketStream; }
    const PacketStreamType &getPacketStream() const{ return mPacketStream; }

private:
    PacketStreamType &mPacketStream;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_PACKETSTREAMUNDERPACKETSTREAM_H
