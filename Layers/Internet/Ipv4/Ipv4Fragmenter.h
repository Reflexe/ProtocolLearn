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

#ifndef PROTOCOLLEARN_IPV4_IPV4FRAGMENTER_H
#define PROTOCOLLEARN_IPV4_IPV4FRAGMENTER_H

#include <list>

#include "Ipv4Packet.h"

namespace ProtocolLearn {
namespace Ipv4 {

class Ipv4Fragmenter
{
public:
    Ipv4Fragmenter();

    // TODO: take all the packets is a waste of memory. we should do it Python-Iterators style.
    static std::list<Ipv4Packet> fragmentPacket(const Ipv4Packet &packetToSplit, OctetVector::SizeType maximumPacketSize);

private:
    static void clearNotRequiredOptions(Ipv4Packet &ipv4Packet);

};

} // namespace Ipv4
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_IPV4_IPV4FRAGMENTER_H
