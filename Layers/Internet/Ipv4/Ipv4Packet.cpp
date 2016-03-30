#include "Ipv4Packet.h"

#include <limits>

#include "InternetChecksum.h"

namespace ProtocolLearn {
namespace Ipv4 {

/**
 * @brief Ipv4Packet::updateChecksum  Update IP checksum in IP header.
 */
void Ipv4Packet::updateChecksum() {
    getHeader().headerChecksum = 0; //During a checksum calculating, checksum field must be set to zero.

    setHeaderChecksum(calculateChecksum(getVectorHeader()));
}

void Ipv4Packet::setInvalidPacket(Ipv4Packet::ParsingError parsingError)
{
    mParsingError = parsingError;
}

uint16_t Ipv4Packet::calculateChecksum(const OctetVector &header) {
    InternetChecksum internetChecksum;

    internetChecksum.add(header);

    return internetChecksum.calculateInternetChecksum();
}

void Ipv4Packet::updateHeaderLength() {
    const auto &newSize = Packet::getHeaderLength();

    pl_assert(newSize % 4 == 0);
    pl_assert((newSize/4) >= 5);
    pl_assert((newSize/4) <= 15);

    if((newSize/4) < 5 || (newSize % 4) != 0)
    {
        return setInvalidPacket(ParsingError::InvalidInternetHeaderLength);
    }

    setInternetHeaderLength(static_cast<uint8_t>(newSize/4));
}

void Ipv4Packet::onPacketExport() {
    getHeader().destinationAddress = ByteOrder::hostToNetwork(mMiniPacket.destination.toInt());
    getHeader().sourceAddress = ByteOrder::hostToNetwork(mMiniPacket.source.toInt());

    importUnrequiredHeader(mOptionsParser.toOctetVector());
    updateHeaderLength();

    pl_assert(getPacketLength() <= std::numeric_limits<uint16_t>::max());
    getHeader().totalLength = ByteOrder::hostToNetwork(static_cast<uint16_t>(getPacketLength()));

    updateChecksum();
}

void Ipv4Packet::fromRawPacket(OctetVector &&rawPacket) {
    OctetVector::SizeType headerSize = BitField<uint8_t>::getPosition<4, 4>(rawPacket.getAsObject<Ipv4Header>().versionAndHeaderLength) * 4;

    if(headerSize > rawPacket.size() || headerSize < getMinimumHeaderLength())
        return PacketWrapper::fromRawPacket(std::move(rawPacket), getMinimumHeaderLength());
    else
        return PacketWrapper::fromRawPacket(std::move(rawPacket), headerSize);
}

Ipv4Packet::Ipv4Packet() {
    setVersion(0x4);                 /* Version                */

    setTimeToLive(0xff);

    updateHeaderLength();
}

void Ipv4Packet::setReservedFlag(bool reserved) {
    getHeader().flagsAndFragmentOffset = ByteOrder::hostToNetwork(
                BitField<uint16_t>::setPositionAndReturn<0, 1>(ByteOrder::networkToHost(getHeader().flagsAndFragmentOffset), reserved)
                );
}

void Ipv4Packet::setDontFragment(bool dontFragment) {
    getHeader().flagsAndFragmentOffset = ByteOrder::hostToNetwork(
                BitField<uint16_t>::setPositionAndReturn<1, 1>(ByteOrder::networkToHost(getHeader().flagsAndFragmentOffset), dontFragment)
                );
}

void Ipv4Packet::setMoreFragments(bool moreFragments) {
    getHeader().flagsAndFragmentOffset = ByteOrder::hostToNetwork(
                BitField<uint16_t>::setPositionAndReturn<2, 1>(ByteOrder::networkToHost(getHeader().flagsAndFragmentOffset), moreFragments)
                );
}

void Ipv4Packet::setFragmentOffset(uint16_t fragmentOffset) {
    getHeader().flagsAndFragmentOffset = ByteOrder::hostToNetwork(
                BitField<uint16_t>::setPositionAndReturn<3, 13>(ByteOrder::networkToHost(getHeader().flagsAndFragmentOffset), fragmentOffset)
                );
}

bool Ipv4Packet::isChecksumValid() const{
    auto header = getVectorHeader();

    header.getAsObject<Ipv4Header>().headerChecksum = 0;

    return getHeaderChecksum() == calculateChecksum(header);
}

void Ipv4Packet::onPacketImport() {
    auto internetHeaderLength = getInternetHeaderLength();

    mParsingError = ParsingError::None;

    if(getVersion() != 4)
        return setInvalidPacket(ParsingError::InvalidVersion);

    if(internetHeaderLength < 5 || internetHeaderLength*4u != getHeaderLength())
        return setInvalidPacket(ParsingError::InvalidInternetHeaderLength);

    mMiniPacket.destination = ByteOrder::networkToHost(getHeader().destinationAddress);
    mMiniPacket.source = ByteOrder::networkToHost(getHeader().sourceAddress);

    if(internetHeaderLength > 5 && isUnrequiredHeaderPacket())
        mOptionsParser.parse(getUnrequiredHeaderBegin(), getUnrequiredHeaderEnd());
    else
        mOptionsParser.clearOptions();
}

} // ProtocolLearn
} // Ipv4
