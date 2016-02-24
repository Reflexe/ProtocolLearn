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

#ifndef DATASTREAMUNDERDATASTREAM_H
#define DATASTREAMUNDERDATASTREAM_H

#include "DataStream.h"

namespace ProtocolLearn {

class DataStreamUnderDataStream : public DataStream
{
public:
    explicit DataStreamUnderDataStream(DataStream &dataStream);

    // We don't want two streams that receive from the same stream.
    DataStreamUnderDataStream(const DataStreamUnderDataStream &) = delete;
    DataStreamUnderDataStream &&operator =(const DataStreamUnderDataStream &) = delete;

    DataStreamUnderDataStream(DataStreamUnderDataStream &&) = default;
    DataStreamUnderDataStream &operator =(DataStreamUnderDataStream &&) = default;

    // DataStream original functions.
    virtual void setTimeout(const Timeout::TimeType &time) override;
    virtual void setMinimumReceiveDataSize(OctetVector::SizeType minimumDataSize) override;

    virtual OctetVector _recv() override;
    virtual void _send(OctetVector &&data) override;

    DataStream &getDataStream() const;

private:
    DataStream &dataStream;
};

}

#endif // DATASTREAMUNDERDATASTREAM_H
