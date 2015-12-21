/**
 * @file arpethernetipv4Filter.h
 * @author shrek0 (shrek0.tk@gmail.com)
  ArpEthernetIpv4Filter
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

#ifndef ARPETHERNETIPV4FILTER_H
#define ARPETHERNETIPV4FILTER_H

#include "ProtocolFilter.h"

#include "ArpEthernetIpv4Packet.h"

namespace ProtocolLearn {
namespace Arpv4 {

class ArpEthernetIpv4Filter : public ProtocolFilter<ArpEthernetIpv4Packet>
{
public:
    enum ProtocolDropReason{
        InvalidTragetIpv4Address,
        InvalidSenderIpv4Address,
        InvalidSenderMacAddress,
        InvalidTargetMacAddress
    };

    ArpEthernetIpv4Filter();

    virtual void filterByPacket(const ArpEthernetIpv4Packet &filteringPacket) override final;

private:
    struct ArpEthernetIpv4Session{
        Ipv4Address targetIpv4Address;
        MacAddress targetMacAddress;
        Ipv4Address senderIpv4Address;
        MacAddress senderMacAddress;
    } mSession;

protected:
    virtual DropReasonType checkByPreviousPacket(const ArpEthernetIpv4Packet &filteredPacket) override final;

};

} // Arpv4
} // ProtocolLearn

#endif // ARPETHERNETIPV4FILTER_H
