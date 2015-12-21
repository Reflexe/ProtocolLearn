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

#ifndef PROTOCOLLEARN_UDPFILTER_H
#define PROTOCOLLEARN_UDPFILTER_H

#include "ProtocolFilter.h"

#include "UdpPacket.h"

namespace ProtocolLearn {
namespace Udp {

class UdpFilter : public ProtocolFilter<UdpPacket>
{
public:
    enum ProtocolDropReason {
        InvalidChecksum,
        InvalidSourcePort,
        InvalidDestinationPort
    };

    UdpFilter();

    bool isInvalidChecksumCritical() const;
    void setIsInvalidChecksumCritical(bool isInvalidChecksumCritical);

    bool isIncorrectSourcePortCritical() const;
    void setIsIncorrectSourcePortCritical(bool isIncorrectSourcePortCritical);

    struct UdpSession{
        uint16_t sourcePort = 0;
        uint16_t destinationPort = 0;
    };

    virtual void filterByPacket(const UdpPacket &filteringPacket) override final;

private:
    virtual DropReasonType checkByProtocol(const UdpPacket &filteredPacket) override final;
    virtual DropReasonType checkByPreviousPacket(const UdpPacket &filteredPacket) override final;

    bool mIsInvalidChecksumCritical = false;
    bool mIsIncorrectSourcePortCritical = false;

    UdpSession mUdpSession;
};

} // namespace Udp
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_UDPFILTER_H
