/**
 * @file arpdatastream.cpp
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.	If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 */

#include "ArpDataStream.h"

namespace ProtocolLearn {
namespace Arp {

ArpDataStream::ArpDataStream(ArpStream &arpStream, uint16_t hardwareFormat, uint16_t protocolFormat, uint8_t hardwareLength, uint8_t protocolLength, uint16_t opcode)
    : DataStreamUnderPacketStream(arpStream) {

    mSendPacket.setHardwareAddressFormat(hardwareFormat);
    mSendPacket.setHardwareAddressLength(hardwareLength);
    mSendPacket.setOpcode(opcode);
    mSendPacket.setProtocolAddressFormat(protocolFormat);
    mSendPacket.setProtocolAddressLength(protocolLength);

    getPacketStream().getFilter().filterByPacket(mSendPacket);
}

} // Arp
} // ProtocolLearn
