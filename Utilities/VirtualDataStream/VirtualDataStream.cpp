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

#include "VirtualDataStream.h"

namespace ProtocolLearn {

VirtualDataStream::VirtualDataStream()
{
}

void VirtualDataStream::_send(OctetVector &&data)
{
    mSendQueue.push(std::move(data));
}

OctetVector VirtualDataStream::_recv(const Timeout &) {
    pl_assert(mReceiveQueue.empty() == false);
    if (mReceiveQueue.empty())
        return OctetVector{};

    OctetVector octetVector = std::move(mReceiveQueue.front());
    mReceiveQueue.pop();

    return octetVector;
}

void VirtualDataStream::insertData(OctetVector &&data)
{
    mReceiveQueue.push(std::move(data));
}

void VirtualDataStream::insertData(const OctetVector &data)
{
    mReceiveQueue.push(OctetVector{data});
}

OctetVector VirtualDataStream::popSentData() {
    OctetVector octetVector = std::move(mSendQueue.front());
    mSendQueue.pop();

    return octetVector;
}


} // namespace ProtocolLearn

