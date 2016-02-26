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

#ifndef ICMPFILTER_H
#define ICMPFILTER_H

#include <memory>

#include "IcmpPacket.h"
#include "Ipv4Filter.h"

#include "ProtocolFilter.h"

namespace ProtocolLearn {
namespace Icmp {

class IcmpFilter : public ProtocolFilter<IcmpPacket>
{
public:
    IcmpFilter();

    struct IcmpSession{
        uint16_t id = 0;
        uint16_t sequence = 0;
        uint16_t type = 0;
        uint16_t code = 0;

        std::array<uint8_t, 8> mIpv4Data;
        OctetVector::SizeType mIpv4DataSize;

        struct Ipv4Session {
            uint32_t sourceAddress;
            uint32_t destinationAddress;
            uint16_t protocol;
            uint16_t identification;
        } mIpv4Session;
    };

    enum ProtocolDropReason{
        InvalidChecksum,
        InvalidSequence,
        InvalidID,
        WrongType,
        InvalidOriginalDatagramSize,
        UnknownResponsePacket
    };

    virtual void filterByPacket(const IcmpPacket &filteringPacket) override final;

    bool isErrorsAccepted() const;
    void setIsErrorsAccepted(bool isErrorsAccepted);

    void setIsErrorsAccepted(bool isErrorsAccepted,
                             const Ipv4::Ipv4Packet &ipv4Packet,
                             const OctetVector &data);

protected:
    virtual DropReasonType checkByProtocol(const IcmpPacket &packet) override final;
    virtual DropReasonType checkByPreviousPacket(const IcmpPacket &filteredPacket) override final;

private:
    bool isError(uint8_t type)
    {
        return type == ICMP_TIME_EXCEEDED || type == ICMP_DEST_UNREACH || type == ICMP_PARAMETERPROB || type == ICMP_REDIRECT;
    }

    bool mIsErrorsAccepted = true;

    IcmpSession mIcmpSession;
};

} // ProtocolLearn
} // Icmp

#endif // ICMPFILTER_H
