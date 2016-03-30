/**
 * @file datastream.cpp
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

#include "DataStream.h"

namespace ProtocolLearn {

DataStream::DataStream()
{
}

void DataStream::sendData(OctetVector &&data)
{
    _send(std::move(data));
}

OctetVector DataStream::receiveData(const Timeout &timeout) {
    do {
        auto data = _recv(timeout);

        pl_debug("checkByMinimumReceiveDataSize: " << (checkByMinimumReceiveDataSize(data) ? "Passed" : "Success"));
       if (checkByMinimumReceiveDataSize(data) == true)
           return data;

       if (timeout.isPassed())
           throw Timeout::TimeoutException("DataStream::receiveData");
    } while(true);
}

void DataStream::setMinimumReceiveDataSize(OctetVector::SizeType minimumDataSize)
{
    mMinimumReceiveDataSize = minimumDataSize;
}

OctetVector::SizeType DataStream::getMinimumReceiveDataSize() const
{
    return mMinimumReceiveDataSize;
}

bool DataStream::checkByMinimumReceiveDataSize(const OctetVector &data) {
    if(mMinimumReceiveDataSize == 0)
        return true;

    return data.size() >= mMinimumReceiveDataSize; //This is the right size?
}

} // ProtocolLearn
