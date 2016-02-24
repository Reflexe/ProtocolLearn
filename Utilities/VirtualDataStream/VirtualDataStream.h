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

#ifndef PROTOCOLLEARN_VIRTUALDATASTREAM_H
#define PROTOCOLLEARN_VIRTUALDATASTREAM_H

#include <queue>

#include "DataStream.h"

namespace ProtocolLearn {

/**
 * @brief A "Data Stream" that basicly does nothing but inserting OctetVectors into Queues.
 */
class VirtualDataStream : public DataStream
{
public:
    VirtualDataStream();
    virtual ~VirtualDataStream() = default;

    virtual void _send(OctetVector &&data) override;
    virtual OctetVector _recv() override;

    /// Insert data to the recieve queue.
    void insertData(OctetVector &&data);
    void insertData(const OctetVector &data);

    /// Pop data from the send queue.
    OctetVector popSentData();

private:
    std::queue<OctetVector> mReceiveQueue;
    std::queue<OctetVector> mSendQueue;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_VIRTUALDATASTREAM_H
