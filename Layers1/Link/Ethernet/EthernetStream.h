#ifndef ETHERNETSTREAM_H
#define ETHERNETSTREAM_H

#include "EthernetPacket.h"
#include "EthernetFilter.h"

#include "BasicInterface.h"

#include "PacketStreamUnderDataStream.h"

namespace ProtocolLearn {
namespace Ethernet {

typedef PacketStreamUnderDataStream<EthernetPacket, EthernetFilter, BasicInterface> EthernetStream;

} // Ethernet
} // ProtocolLearn

#endif // ETHERNETSTREAM_H
