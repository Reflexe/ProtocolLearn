/**
 * @file ArpDataStream.h
 * @author shrek0 (shrek0.tk@gmail.com)
  ArpDataStream
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

#ifndef ARPDATASTREAM_H
#define ARPDATASTREAM_H

#include "ArpStream.h"

#include "DataStreamUnderPacketStream.h"

namespace ProtocolLearn {
namespace Arp {

class ArpDataStream : public DataStreamUnderPacketStream<ArpStream>
{
public:
    ArpDataStream(ArpStream &arpStream, uint16_t hardwareFormat, uint16_t protocolFormat,
                  uint8_t hardwareLength, uint8_t protocolLength, uint16_t opcode);
};

} // Arp
} // ProtocolLearn

#endif // ARPDATASTREAM_H
