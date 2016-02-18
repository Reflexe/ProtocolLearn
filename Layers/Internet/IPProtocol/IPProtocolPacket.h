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

#ifndef PROTOCOLLEARN_IPPROTOCOLPACKET_H
#define PROTOCOLLEARN_IPPROTOCOLPACKET_H

#include "Packet.h"

namespace ProtocolLearn {

class IPProtocolPacket : public Packet
{
public:
    static const OctetVector::SizeType MinimumHeaderLength = 0;

    IPProtocolPacket(OctetVector::SizeType headerSize);

    const OctetVector &getPseudoHeader() const;
    OctetVector &getPseudoHeader();

private:
    OctetVector mPseudoHeader;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_IPPROTOCOLPACKET_H
