/**
 * @file netlinkFilter.h
 * @author shrek0 (shrek0.tk@gmail.com)
  NetlinkFilter
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

#ifndef NETLINKFILTER_H
#define NETLINKFILTER_H

#include "ProtocolFilter.h"

#include "NetlinkPacket.h"

namespace ProtocolLearn {
namespace Netlink {

class NetlinkFilter : public ProtocolFilter<NetlinkPacket>
{
public:
    NetlinkFilter();

    enum ProtocolDropReason{
        NoData
    };

protected:
    virtual DropReasonType checkByProtocol(const NetlinkPacket &filteredPacket) override final;

};

} // ProtocolLearn
} // Netlink

#endif // NETLINKFILTER_H