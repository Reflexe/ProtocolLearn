#ifndef ARPFILTER_H
#define ARPFILTER_H

#include "ProtocolFilter.h"

#include "ArpPacket.h"

namespace ProtocolLearn {

namespace Arp {

class ArpFilter : public ProtocolFilter<ArpPacket>
{
public:
    ArpFilter();
    virtual ~ArpFilter() = default;

    ArpHeader previousPacket;

    enum ProtocolDropReason{
        InvalidHardwareFormat,
        InvalidProtocolFormat,
        InvalidHardwareLength,
        InvalidProtocolLength,
        InvalidOpcode,
        NoData
    };

    virtual void filterByPacket(const ArpPacket &filteringPacket) override final;

protected:
    virtual DropReasonType checkByProtocol(const ArpPacket &filteredPacket) override final;
    virtual DropReasonType checkByPreviousPacket(const ArpPacket &filteredPacket) override final;
};

} // Arp

} // ProtocolLearn

#endif // ARPFILTER_H
