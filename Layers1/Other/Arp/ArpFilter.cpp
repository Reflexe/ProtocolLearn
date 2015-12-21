#include "ArpFilter.h"

namespace ProtocolLearn {

namespace Arp {

ArpFilter::ArpFilter()
{
}

ArpFilter::DropReasonType ArpFilter::checkByProtocol(const ArpPacket &filteredPacket) {  // This packet must has data.
    if(filteredPacket.isDataPacket() == false)
        return NoData;

    return DropReason::None;
}

ArpFilter::DropReasonType ArpFilter::checkByPreviousPacket(const ArpPacket &filteredPacket) {
    if(filteredPacket.getHardwareAddressLength() != previousPacket.hardwareLength)
        return InvalidHardwareLength;
    if(filteredPacket.getHardwareAddressFormat() != previousPacket.hardwareFormat)
        return InvalidHardwareFormat;
    if(filteredPacket.getProtocolAddressFormat() != previousPacket.protocolFormat)
        return InvalidProtocolFormat;
    if(filteredPacket.getProtocolAddressLength() != previousPacket.protocolLength)
        return InvalidProtocolLength;
    if(filteredPacket.getOpcode() == ARPOP_REPLY && previousPacket.opcode != ARPOP_REQUEST)
        return InvalidOpcode;

    return DropReason::None;
}

void ArpFilter::filterByPacket(const ArpPacket &filteringPacket) {
    previousPacket.hardwareFormat = filteringPacket.getHardwareAddressFormat();
    previousPacket.hardwareLength = filteringPacket.getHardwareAddressLength();
    previousPacket.protocolFormat = filteringPacket.getProtocolAddressFormat();
    previousPacket.protocolLength = filteringPacket.getProtocolAddressLength();
    previousPacket.opcode = filteringPacket.getOpcode();
}

} // Arp

} // ProtocolLearn
