/**
 * @file
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
 *
 */

#include "TcpPacket.h"
#include "InternetChecksum.h"

namespace ProtocolLearn {
namespace Tcp {

TcpPacket::TcpPacket()
{
}

void TcpPacket::parseOptions(OctetVector::SizeType dataOffset) {
    pl_debug("Parsering options...");

    auto dataOffestInDataVector = dataOffset - getMinimumHeaderLength();

    if(dataOffestInDataVector > getDataLength())
        return setInvalidPacket(ParsingError::NoPlaceForOptionsInData);

    // Parse the options.
    OctetVector unrequiredHeader{getVectorData().cbegin(), getVectorData().cbegin() + dataOffestInDataVector};

    // erase the options data (from the data vector).
    getVectorData().erase(getVectorData().begin(), getVectorData().begin() + dataOffestInDataVector);

    mOptionsParser.parse(unrequiredHeader);
    importUnrequiredHeader(unrequiredHeader);
}

void TcpPacket::onPacketImport() {
    pl_debug("Parsering a new packet...");

    OctetVector::SizeType dataOffset = getOffset()*4;

    if(getMinimumHeaderLength() > dataOffset)
        return setInvalidPacket(ParsingError::OffsetLowerThanMinimumHeader);

    if(dataOffset > getMinimumHeaderLength())
        parseOptions(dataOffset);
    else
        getParser().clearOptions();
}

void TcpPacket::onPacketExport() {
    importUnrequiredHeader(mOptionsParser.toOctetVector());

    setOffset(static_cast<uint8_t>(getHeaderLength()/4));

    getHeader().checksum = 0;
    setChecksum(calculateChecksum(getPseudoHeader(), getVectorHeader(), getVectorData()));
}

void TcpPacket::setInvalidPacket(TcpPacket::ParsingError parsingError)
{
    mParsingError = parsingError;
}

bool TcpPacket::isChecksumValid() const{
    auto tcpVectorHeader = getVectorHeader();

    tcpVectorHeader.getAsObject<TcpHeader>().checksum = 0;

    return calculateChecksum(getPseudoHeader(), tcpVectorHeader, getVectorData()) == getChecksum();
}

OctetVector::SizeType TcpPacket::getSegementLength() const{
    OctetVector::SizeType segementLength = getDataLength();

    if(getFinishFlag() || getSynFlag())
        segementLength += 1;

    return segementLength;
}

uint16_t TcpPacket::calculateChecksum(const OctetVector &pseudoHeader, const OctetVector &header, const OctetVector &data) {
    InternetChecksum internetChecksum;

    internetChecksum.add(pseudoHeader);
    internetChecksum.add(header);
    internetChecksum.add(data);

    // The header and the pseudo header must be aligned anyway.
    if(data.size() % 2 != 0)
        internetChecksum.add(0);

    return internetChecksum.calculateInternetChecksum();
}

} // Tcp
} // ProtocolLearn
