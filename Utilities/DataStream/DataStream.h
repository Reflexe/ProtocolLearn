/**
 * @file DataStream.h
 * @author shrek0 (shrek0.tk@gmail.com)
  DataStream
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

#ifndef DATASTREAM_H
#define DATASTREAM_H

#include "Timeout.h"
#include "OctetVector.h"

namespace ProtocolLearn {

class DataStream
{
public:
    DataStream();
    virtual ~DataStream() = default;

    virtual void sendData(OctetVector &&data);
    virtual OctetVector receiveData();

    OctetVector _receiveData();

    virtual void _send(OctetVector &&data) = 0;
    virtual OctetVector _recv() = 0;

    virtual void setMinimumReceiveDataSize(OctetVector::SizeType minimumDataSize);
    OctetVector::SizeType getMinimumReceiveDataSize() const;

    virtual void setTimeout(const Timeout::TimeType &timeout);

    const PTime &getTimeout() const;

protected:
    bool checkByMinimumReceiveDataSize(const OctetVector &data);

    OctetVector::SizeType mMinimumReceiveDataSize = 1;
    Timeout mTimeout{PTime::infinity()};
};

} // ProtocolLearn

#endif // DATASTREAM_H
