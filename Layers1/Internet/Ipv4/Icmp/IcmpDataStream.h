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

#ifndef PROTOCOLLEARN_ICMP_ICMPDATASTREAM_H
#define PROTOCOLLEARN_ICMP_ICMPDATASTREAM_H

#include "DataStreamUnderPacketStream.h"

#include "IcmpStream.h"
#include "BasicProtocol.h"

namespace ProtocolLearn {
namespace Icmp {

/**
 * @brief The IcmpDataStream class  The only way to use it is to tunnel data under Icmp pings.
 */
class IcmpDataStream : public DataStreamUnderPacketStream<IcmpStream, BasicProtocol>
{
public:
    IcmpDataStream(IcmpStream &icmpStream, uint8_t type, uint8_t code=0);

    virtual void sendData(const OctetVector &data) override final;

    OctetVector::SizeType performMaxTransmissionUnitPathDiscovery(const OctetVector::SizeType &minimumAccurecy, const uint16_t maxFails);
};

} // namespace Icmp
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_ICMP_ICMPDATASTREAM_H
