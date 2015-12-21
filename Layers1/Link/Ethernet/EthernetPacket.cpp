#include "EthernetPacket.h"

namespace ProtocolLearn {
namespace Ethernet {

void EthernetPacket::onPacketImport() {
    ethernetMiniPacket.destination = getHeader().destination;
    ethernetMiniPacket.source = getHeader().source;
}

void EthernetPacket::onPacketExport() {
    ethernetMiniPacket.destination.copyNumeric(getHeader().destination);
    ethernetMiniPacket.source.copyNumeric(getHeader().source);
}

EthernetPacket::EthernetPacket()
{
}

} // Ethernet
} // ProtocolLearn
