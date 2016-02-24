#include "Ipv4Stream.h"

#include "Ipv4Fragmenter.h"

namespace ProtocolLearn {
namespace Ipv4 {

Ipv4Stream::Ipv4Stream(LinkProtocol &linkProtocol)
    : PacketStreamUnderDataStream{linkProtocol}
{
}

} // ProtocolLearn
} // Ipv4
