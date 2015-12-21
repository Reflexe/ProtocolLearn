#ifndef ETHERNETFILTER_H
#define ETHERNETFILTER_H

#include "ProtocolFilter.h"

#include "EthernetPacket.h"

namespace ProtocolLearn {
namespace Ethernet {

/**
 * @brief The EthernetFilter class
 */
class EthernetFilter : public ProtocolFilter<EthernetPacket>
{
public:
    EthernetFilter();

    enum ProtocolDropReason{
        InvalidProtocol,
        InvalidSource,
        InvalidDestination
    };

    virtual void filterByPacket(const EthernetPacket &filteringPacket) override final;

protected:
    virtual DropReasonType checkByPreviousPacket(const EthernetPacket &filteredPacket) override final;

private:
    struct EthernetFilterHeader {
        MacAddress destination;
        MacAddress source;
        uint16_t protocol = 0;
    };

    /**
     * @brief mPreviousPacket
     */
    EthernetFilterHeader mPreviousPacket;
};

} // Ethernet
} // ProtocolLearn

#endif // ETHERNETFILTER_H
