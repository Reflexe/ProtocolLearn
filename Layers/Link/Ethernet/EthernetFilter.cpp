#include "EthernetFilter.h"

namespace ProtocolLearn {
namespace Ethernet {

EthernetFilter::EthernetFilter()
{
}

EthernetFilter::DropReasonType EthernetFilter::checkByPreviousPacket(const EthernetPacket &filteredPacket) {
    if(filteredPacket.getProtocol() != mPreviousPacket.protocol)
        return InvalidProtocol;
    if(filteredPacket.getDestination() != mPreviousPacket.source)
        return InvalidDestination;
    if(filteredPacket.getSource() != mPreviousPacket.destination
            && mPreviousPacket.destination != MacAddress::BroadcastAddress)
        return InvalidSource;

    return DropReason::None;
}

void EthernetFilter::filterByPacket(const EthernetPacket &filteringPacket) {
    mPreviousPacket.destination = filteringPacket.getDestination();
    mPreviousPacket.source = filteringPacket.getSource();
    mPreviousPacket.protocol = filteringPacket.getProtocol();
}

} // Ethernet
} // ProtocolLearn
