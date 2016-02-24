#ifndef IPV4PACKET_H
#define IPV4PACKET_H

#include "Ipv4Address.h"

#include "PacketWrapper.h"
#include "ByteOrder.h"

#include "BitField.h"

#include "Ipv4OptionsParser.h"

namespace ProtocolLearn {
namespace Ipv4 {

struct Ipv4Header{
    uint8_t versionAndHeaderLength;
    uint8_t DSCPAndECN;
    uint16_t totalLength;
    uint16_t identification;
    uint16_t flagsAndFragmentOffset;
    uint8_t timeToLive;
    uint8_t protocol;
    uint16_t headerChecksum;
    uint32_t sourceAddress;
    uint32_t destinationAddress;
}__attribute__((packed));

class Ipv4Packet : public PacketWrapper<Ipv4Header>
{
public:
    Ipv4Packet();

    /* Header settings */
    void setSource(const Ipv4Address &source) { mMiniPacket.source = source; }
    void setDestination(const Ipv4Address &destination) { mMiniPacket.destination = destination; }

    void setVersion(uint8_t version) { BitField<uint8_t>::setPosition<0, 4>(getHeader().versionAndHeaderLength, version); }
    void setInternetHeaderLength(uint8_t internetHeaderLength) { BitField<uint8_t>::setPosition<4, 4>(getHeader().versionAndHeaderLength, internetHeaderLength); }
    void setHeaderChecksum(uint16_t checksum) { getHeader().headerChecksum = ByteOrder::hostToNetwork(checksum); }
    void setDSCP(uint8_t DSCP) { BitField<uint8_t>::setPosition<0, 6>(getHeader().DSCPAndECN, DSCP); }
    void setECN(uint8_t ECN) { BitField<uint8_t>::setPosition<6, 2>(getHeader().DSCPAndECN, ECN); ; }
    void setTotalLength(uint16_t totalLength) { getHeader().totalLength = ByteOrder::hostToNetwork(totalLength); }
    void setIdentification(uint16_t identification) { getHeader().identification = ByteOrder::hostToNetwork(identification); }
    void setReservedFlag(bool reserved);
    void setDontFragment(bool dontFragment);
    void setMoreFragments(bool moreFragments);
    void setFragmentOffset(uint16_t fragmentOffset);
    void setTimeToLive(uint8_t timeToLive) { getHeader().timeToLive = timeToLive; }
    void setProtocol(uint8_t protocol) { getHeader().protocol = protocol; }

    /* Get header settings */
    const Ipv4Address &getSource() const{ return mMiniPacket.source; }
    const Ipv4Address &getDestination() const{ return mMiniPacket.destination; }

    uint8_t getVersion() const{ return BitField<uint8_t>::getPosition<0, 4>(getHeader().versionAndHeaderLength); }
    uint8_t getInternetHeaderLength() const{ return BitField<uint8_t>::getPosition<4, 4>(getHeader().versionAndHeaderLength); }
    uint8_t getDSCP() const{ return BitField<uint8_t>::getPosition<0, 6>(getHeader().DSCPAndECN); }
    uint8_t getECN() const{return BitField<uint8_t>::getPosition<6, 2>(getHeader().DSCPAndECN); }
    uint16_t getTotalLength() const{ return ByteOrder::networkToHost(getHeader().totalLength); }
    uint16_t getIdentification() const{ return ByteOrder::networkToHost(getHeader().identification); }
    bool getReservedFlag() const{ return BitField<uint16_t>::getPosition<0, 1>(ByteOrder::networkToHost(getHeader().flagsAndFragmentOffset)); }
    bool getDontFragmentFlag() const{ return BitField<uint16_t>::getPosition<1, 1>(ByteOrder::networkToHost(getHeader().flagsAndFragmentOffset)); }
    bool getMoreFragmentsFlag() const{ return BitField<uint16_t>::getPosition<2, 1>(ByteOrder::networkToHost(getHeader().flagsAndFragmentOffset)); }
    uint16_t getFragmentOffset() const{ return BitField<uint16_t>::getPosition<3, 13>(ByteOrder::networkToHost(getHeader().flagsAndFragmentOffset)); }
    uint8_t getTimeToLive() const{ return getHeader().timeToLive; }
    uint8_t getProtocol() const{ return getHeader().protocol; }
    uint16_t getHeaderChecksum() const{ return ByteOrder::networkToHost(getHeader().headerChecksum); }

    Ipv4OptionsParser &getParser() { return mOptionsParser; }
    const Ipv4OptionsParser &getParser() const{ return mOptionsParser; }

    virtual void fromRawPacket(OctetVector &&rawPacket) override final;

    bool isChecksumValid() const;
    bool isValidPacket() const{ return mParsingError == ParsingError::None; }

    enum class ParsingError {
        None,
        InvalidVersion,
        InvalidInternetHeaderLength,
        NoPlaceForOptions
    };

    ParsingError getParsingError() const{ return mParsingError; }

protected:
    virtual void onPacketImport() override final;
    virtual void onPacketExport() override final;

private:
    void updateHeaderLength();
    void updateChecksum();
    void setInvalidPacket(ParsingError parsingError);

    static uint16_t calculateChecksum(const OctetVector &header);

    struct IPv4MiniPacket{
        Ipv4Address source;
        Ipv4Address destination;
    };

    IPv4MiniPacket mMiniPacket;
    Ipv4OptionsParser mOptionsParser;
    ParsingError mParsingError = ParsingError::None;
};

} // Ipv4
} // ProtocolLearn

#endif // IPV4PACKET_H
