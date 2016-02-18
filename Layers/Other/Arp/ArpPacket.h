#ifndef ARPPACKET_H
#define ARPPACKET_H

#include <net/if_arp.h>

#include "PacketWrapper.h"
#include "ByteOrder.h"

namespace ProtocolLearn {

namespace Arp {

// Taken from net/if_arp.h
struct ArpHeader {
    uint16_t hardwareFormat;		/* Format of hardware address.  */
    uint16_t protocolFormat;		/* Format of protocol address.  */
    uint8_t hardwareLength;		/* Length of hardware address.  */
    uint8_t protocolLength;		/* Length of protocol address.  */
    uint16_t opcode;		/* ARP opcode (command).  */
}__attribute__((packed));

class ArpPacket : public PacketWrapper<ArpHeader>
{
public:
    ArpPacket();

    void setHardwareAddressFormat(uint16_t format) { getHeader().hardwareFormat = ByteOrder::hostToNetwork(format); }
    void setProtocolAddressFormat(uint16_t format) { getHeader().protocolFormat = ByteOrder::hostToNetwork(format); }

    void setHardwareAddressLength(uint8_t length) { getHeader().hardwareLength = length; }
    void setProtocolAddressLength(uint8_t length) { getHeader().protocolLength = length; }

    void setOpcode(uint16_t opcode) { getHeader().opcode = ByteOrder::hostToNetwork(opcode); }

    uint16_t getHardwareAddressFormat() const{ return ByteOrder::networkToHost(getHeader().hardwareFormat); }
    uint16_t getProtocolAddressFormat() const{ return ByteOrder::networkToHost(getHeader().protocolFormat); }

    // This is one byte, no need to convert to host byte order.
    uint8_t getHardwareAddressLength() const{ return getHeader().hardwareLength; }
    uint8_t getProtocolAddressLength() const{ return getHeader().protocolLength; }

    uint16_t getOpcode() const{ return ByteOrder::networkToHost(getHeader().opcode); }
};

} // Arp

} // ProtocolLearn


#endif // ARPPACKET_H
