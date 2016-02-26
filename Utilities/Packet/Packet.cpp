/**
 * @file packet.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) 2015 shrek0
 *
 * ProtocolLearn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProtocolLearn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 * Contain a class that provide easy way to packet managing.
 */

#include "Packet.h"

#include <limits>

#include "Exceptions.h"

namespace ProtocolLearn {

void Packet::init() {
    resizeHeader(minimumHeaderLength);
    resizeData(0);
}

void Packet::importHeader(const OctetVector &dataType) {
    pl_assert(dataType.size() > minimumHeaderLength);
    pl_assert(isValidSizeType(dataType.size()));

    d.header = dataType;
}

void Packet::importUnrequiredHeader(const OctetVector &dataType) {
    pl_assert(isValidSizeType(dataType.size()));
    pl_assert(isValidSizeType(dataType.size()+getMinimumHeaderLength()));

    resizeHeader(getMinimumHeaderLength());

    d.header.add(dataType);
}

OctetVector Packet::getUnrequiredHeader() {
    if(getHeaderLength() == getMinimumHeaderLength())
        return OctetVector{};

    return OctetVector(getVectorHeader().cbegin()+getMinimumHeaderLength(), getVectorHeader().cend());
}

void Packet::fromRawPacket(OctetVector &&rawPacket, OctetVector::SizeType headerLength) {
    pl_assert(headerLength >= getMinimumHeaderLength());
    pl_assert(rawPacket.size() >= getMinimumHeaderLength());
    pl_assert(rawPacket.size() >= headerLength);

    pl_assert(isValidSizeType(headerLength));

    d.header = std::move(rawPacket);

    if (rawPacket.size() != headerLength) {
        auto dataIterator = d.header.begin()+headerLength;

        pl_assert(isValidSizeType(d.header.size()-headerLength));

        d.data.assign(dataIterator, d.header.end());
        d.header.resize(headerLength); // Remove thed aata from the header.
    }

    onPacketImport();
}

void Packet::fromRawPacket(OctetVector &&rawPacket)
{
    return fromRawPacket(std::move(rawPacket), getMinimumHeaderLength());
}

void Packet::fromPacket(Packet &packet)
{
    fromRawPacket(packet.toVectorRawPacket(), packet.getHeaderLength());
}

OctetVector Packet::toVectorRawPacket() {
    onPacketExport();

    return toOctetVector();
}

void Packet::onExport()
{
    onPacketExport();
}

void Packet::importData(OctetVector &&data) {
    d.data = std::move(data);

    pl_assert(isValidSizeType(data.size()));
}

OctetVector Packet::toOctetVector() const{
    OctetVector rawPacket = d.header;

    // Write the data into the vector.
    rawPacket.add(d.data);

    return rawPacket;
}

void Packet::importData(const OctetVector &data) {
    removeData();

    pl_assert(isValidSizeType(data.size()));

    d.data = data;
}

void Packet::importData(Packet &packet)
{
    importData(packet.toVectorRawPacket());
}

void Packet::onPacketExport() { }
void Packet::onPacketImport() { }

bool Packet::isValidSizeType(const OctetVector::SizeType &dataSizeType) {
    // Protect from integer overflow, sometimes we add OctetVector::SizeType to another OctetVector::SizeType.
    // Therefore, we deny request that grather than max_size/2.

    if(dataSizeType <= std::numeric_limits<OctetVector::SizeType>::max()/2)
        return true;
    else {
        pl_debug("Not valid size:" << dataSizeType);

        return false;
    }
}

Packet::Packet(const OctetVector::SizeType &minimumHeaderLength)
    : minimumHeaderLength(minimumHeaderLength)
{
    init();
}

void Packet::reset() {
    d.data.clear();
    d.header.clear();

    init();
}

void Packet::removeData()
{
    resizeData(0);
}

}
