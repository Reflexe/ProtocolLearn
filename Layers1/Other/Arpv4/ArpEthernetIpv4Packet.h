#ifndef ARPETHERNETIPV4PACKET_H
#define ARPETHERNETIPV4PACKET_H

#include "PacketWrapper.h"
#include "ByteOrder.h"

#include "MacAddress.h"
#include "Ipv4Address.h"

#include <net/if_arp.h>

namespace ProtocolLearn {

namespace Arpv4 {

struct ArpEthernetIpv4Header {
    uint8_t senderMacAddress[MacAddress::MacAddressLength];	/* Sender Mac address.   */
    uint32_t senderIpv4Address;            /* Sender IPv4 address.  */
    uint8_t targetMacAddress[MacAddress::MacAddressLength];	/* Target Mac address.   */
    uint32_t targetIpv4Address;            /* Target IPv4 address.  */
}__attribute__((packed));

class ArpEthernetIpv4Packet : public PacketWrapper<ArpEthernetIpv4Header>
{
public:
//    typedef uint64_t BiggestType;
//    static const Packet::DataSizeType minimumHeaderLength = sizeof(ArpEthernetIpv4Header);

    ArpEthernetIpv4Packet();

    void setSenderMacAddress(const MacAddress &macAddress) { arpMiniPacket.senderMacAddress = macAddress; }
    void setTargetMacAddress(const MacAddress &macAddress) { arpMiniPacket.targetMacAddress = macAddress; }

    void setSenderIpv4Address(const Ipv4Address &ipv4Address) { arpMiniPacket.senderIpv4Address = ipv4Address; }
    void setTargetIpv4Address(const Ipv4Address &ipv4Address) { arpMiniPacket.targetIpv4Address = ipv4Address; }

    const MacAddress &getSenderMacAddress() const{ return arpMiniPacket.senderMacAddress; }
    const MacAddress &getTargetMacAddress() const{ return arpMiniPacket.targetMacAddress; }

    const Ipv4Address &getSenderIpv4Address() const{ return arpMiniPacket.senderIpv4Address; }
    const Ipv4Address &getTargetIpv4Address() const{ return arpMiniPacket.targetIpv4Address; }

protected:
    virtual void onPacketImport() override final;
    virtual void onPacketExport() override final;

private:
    struct ArpEthernetIpv4PacketMiniPacket{
        MacAddress senderMacAddress;
        MacAddress targetMacAddress;

        Ipv4Address senderIpv4Address;
        Ipv4Address targetIpv4Address;
    };

    ArpEthernetIpv4PacketMiniPacket arpMiniPacket;
};

} // Arpv4



} // ProtocolLearn

#endif // ARPETHERNETIPV4PACKET_H
