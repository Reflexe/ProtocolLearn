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

#ifndef DERIVEDDATASTREAM_H
#define DERIVEDDATASTREAM_H

#include "DataStreamUnderDataStream.h"
#include "MultiStreamDataStream.h"

namespace ProtocolLearn {

class DerivedDataStream : public DataStreamUnderDataStream
{
public:
    explicit DerivedDataStream(MultiStreamDataStream &multiStreamDataStream);
    ~DerivedDataStream();

    virtual void _recv(OctetVector &data) override;

    void stopCounting();
    void continueCounting();

private:
    PacketCache::PacketID savedPacketID = 0;

    MultiStreamDataStream::StreamID streamID;
    MultiStreamDataStream &multiStreamDataStream;

    void setTimeout(time_t seconds, useconds_t useconds) override;
    void setMinimumReceiveDataSize(OctetVector::SizeType minimumDataSize) override;
};

}

#endif // DERIVEDDATASTREAM_H
