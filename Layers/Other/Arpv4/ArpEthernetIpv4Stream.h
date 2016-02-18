#ifndef ARPETHERNETIPV4STREAM_H
#define ARPETHERNETIPV4STREAM_H

#include "PacketStreamUnderDataStream.h"

#include "ArpEthernetIpv4Packet.h"
#include "ArpEthernetIpv4Filter.h"

namespace ProtocolLearn {
namespace Arpv4 {

typedef PacketStreamUnderDataStream<ArpEthernetIpv4Packet, ArpEthernetIpv4Filter> ArpEthernetIpv4Stream;

} // Arpv4
} // ProtocolLearn

#endif // ARPETHERNETIPV4STREAM_H
