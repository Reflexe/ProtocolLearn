#ifndef IPV4STREAM_H
#define IPV4STREAM_H

#include "PacketStreamUnderDataStream.h"

#include "Ipv4Filter.h"
#include "Ipv4FragmentReassembler.h"

#include "LinkProtocol.h"

namespace ProtocolLearn {

namespace Ipv4 {

class Ipv4Stream : public PacketStreamUnderDataStream<Ipv4Packet, Ipv4Filter, LinkProtocol>
{
public:
    Ipv4Stream(LinkProtocol &linkProtocol);

//    /**
//     * @brief receivePacket  Receive an Ipv4Packet, if the packet is fragmented,
//     * @param packet
//     */
//    virtual void receivePacket(Ipv4Packet &packet) override;

//    /**
//     * @brief sendPacket Send an Ipv4Packet, If the packet's length is grether than the network MTU, the packet will splited and sent as fragments.
//     * @param packet
//     * @note If you want to handle fragmention by yourself, use the sendFragment function.
//     */
//    virtual void sendPacket(Ipv4Packet &packet) override;
};

} // ProtocolLearn

} // Ipv4


#endif // IPV4STREAM_H
