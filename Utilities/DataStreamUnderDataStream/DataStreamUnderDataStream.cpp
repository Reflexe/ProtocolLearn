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

#include "DataStreamUnderDataStream.h"

namespace ProtocolLearn {

DataStreamUnderDataStream::DataStreamUnderDataStream(ProtocolLearn::DataStream &dataStream)
    : dataStream(dataStream)
{
}

OctetVector DataStreamUnderDataStream::_recv(const Timeout &timeout)
{
    return dataStream.receiveData(timeout);
}

void DataStreamUnderDataStream::_send(OctetVector &&data)
{
    dataStream.sendData(std::move(data));
}

DataStream &DataStreamUnderDataStream::getDataStream() const
{
    return dataStream;
}

void DataStreamUnderDataStream::setMinimumReceiveDataSize(OctetVector::SizeType minimumDataSize) {
    if(getMinimumReceiveDataSize() == minimumDataSize)
        return;

    DataStream::setMinimumReceiveDataSize(minimumDataSize);
    dataStream.setMinimumReceiveDataSize(minimumDataSize);
}

}
