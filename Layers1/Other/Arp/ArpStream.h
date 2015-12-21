#ifndef ARPSTREAM_H
#define ARPSTREAM_H

#include "PacketStreamUnderDataStream.h"

#include "ArpPacket.h"
#include "ArpFilter.h"

namespace ProtocolLearn {
namespace Arp {

typedef PacketStreamUnderDataStream<ArpPacket, ArpFilter> ArpStream;

} // Arp
} // ProtocolLearn

#endif // ARPSTREAM_H
