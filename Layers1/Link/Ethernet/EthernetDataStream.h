/**
 * @file EthernetDataStream.h
 * @author shrek0 (shrek0.tk@gmail.com)
  EthernetDataStream
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

#ifndef ETHERNETDATASTREAM_H
#define ETHERNETDATASTREAM_H

#include "DataStreamUnderPacketStream.h"

#include "LinkProtocol.h"

#include "EthernetStream.h"
#include "MacAddress.h"

namespace ProtocolLearn {
namespace Ethernet {

class EthernetDataStream : public DataStreamUnderPacketStream<EthernetStream, LinkProtocol>
{
public:
    EthernetDataStream(EthernetStream &ethernetStream,
                       const MacAddress &destination,
                       uint16_t protocol, const MacAddress &source);

    virtual BasicInterface &getBasicInterface() override;

    virtual std::unique_ptr<LinkProtocolFork> fork(bool reply=false) override;

    virtual OctetVector::SizeType getMaximumSendDataLength() override;
};

} // Ethernet
} // ProtocolLearn

#endif // ETHERNETDATASTREAM_H
