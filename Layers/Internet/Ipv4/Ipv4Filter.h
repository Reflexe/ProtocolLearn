#ifndef IPV4FILTER_H
#define IPV4FILTER_H

#include "Ipv4Packet.h"
#include "ProtocolFilter.h"

#include "Timeout.h"

namespace ProtocolLearn {
namespace Ipv4 {

class Ipv4Filter : public ProtocolFilter<Ipv4Packet>
{
public:
    Ipv4Filter();

    struct Ipv4Session{
        uint32_t sourceAddress;
        uint32_t destinationAddress;

        uint8_t protocol;
    };

    enum ProtocolDropReason{
        InvalidSourceAddress,
        InvalidDestinationAddres,
        InvalidProtocol,
        InvalidVersion,
        InvalidTimeToLive,
        InvalidPacket,
        InvalidOptions,
        InvalidChecksum,
        InvalidTotalLength
    };

    Ipv4Session &getOurSession() { return ourSession; }

    virtual void filterByPacket(const Ipv4Packet &filteringPacket) override final;

protected:
    virtual DropReasonType checkByProtocol(const Ipv4Packet &filteredPacket) override final;
    virtual DropReasonType checkByPreviousPacket(const Ipv4Packet &filteredPacket) override final;

private:
    Ipv4Session ourSession;

};

} // Ipv4
} // ProtocolLearn


#endif // IPV4FILTER_H
