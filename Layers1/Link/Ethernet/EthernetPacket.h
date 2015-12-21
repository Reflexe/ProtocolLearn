#ifndef ETHERNETPACKET_H
#define ETHERNETPACKET_H

#include <net/ethernet.h>

#include "PacketWrapper.h"
#include "ByteOrder.h"

#include "MacAddress.h"

namespace ProtocolLearn {

namespace Ethernet {

struct EthernetHeader{
    uint8_t destination[ETH_ALEN];
    uint8_t source[ETH_ALEN];
    uint16_t protocol;
}__attribute__((packed));

class EthernetPacket : public PacketWrapper<EthernetHeader>
{
public:
    EthernetPacket();

    void setDestination(const MacAddress &destination) { ethernetMiniPacket.destination = destination; }
    void setSource(const MacAddress &source) { ethernetMiniPacket.source = source; }
    void setProtocol(uint16_t protocol) { getHeader().protocol = ByteOrder::hostToNetwork(protocol); }

    const MacAddress &getDestination() const{ return ethernetMiniPacket.destination; }
    const MacAddress &getSource() const{ return ethernetMiniPacket.source; }
    uint16_t getProtocol() const{ return ByteOrder::networkToHost(getHeader().protocol); }

protected:
    virtual void onPacketImport() override final;
    virtual void onPacketExport() override final;

private:
    struct EthernetMiniPacket{
        MacAddress destination;
        MacAddress source;
    };

    EthernetMiniPacket ethernetMiniPacket;
};

} // Ethernet

} // ProtocolLearn

#endif // ETHERNETPACKET_H
