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

#ifndef TCPPACKET_H
#define TCPPACKET_H

#include "Packet.h"
#include "ByteOrder.h"

#include "PacketWrapper.h"

#include "TcpOptionsParser.h"
#include "IPProtocolPacket.h"

namespace ProtocolLearn {
namespace Tcp {

struct TcpHeader{
    uint16_t sourcePort = 0;
    uint16_t destinationPort = 0;
    uint32_t sequenceNumber = 0;
    uint32_t acknowledgmentNumber = 0;
#if BYTE_ORDER == LITTLE_ENDIAN
    bool NSFlag:1;
    uint8_t reserved:3;
    uint8_t offset:4;

    bool finishFlag:1;
    bool synFlag:1;
    bool resetFlag:1;
    bool pushFlag:1;
    bool acknowledgmentFlag:1;
    bool urgentFlag:1;
    bool ECEFlag:1;
    bool CWRFlag:1;
#else
    uint8_t offset:4;
    uint8_t reserved:3;
    bool NSFlag:1;

    bool CWRFlag:1;
    bool ECEFlag:1;
    bool urgentFlag:1;
    bool acknowledgmentFlag:1;
    bool pushFlag:1;
    bool resetFlag:1;
    bool synFlag:1;
    bool finishFlag:1;
#endif
    uint16_t windowSize = 0;
    uint16_t checksum = 0;
    uint16_t urgentPointer = 0;
}__attribute__((packed));

class TcpPacket : public PacketWrapper<TcpHeader, IPProtocolPacket>
{
public:
    TcpPacket();

    uint16_t getSourcePort() const{ return ByteOrder::networkToHost(getHeader().sourcePort); }
    uint16_t getDestinationPort() const{ return ByteOrder::networkToHost(getHeader().destinationPort); }
    uint32_t getSequenceNumber() const{ return ByteOrder::networkToHost(getHeader().sequenceNumber); }
    uint32_t getAcknowledgmentNumber() const{ return ByteOrder::networkToHost(getHeader().acknowledgmentNumber); }
    uint8_t getOffset() const{ return getHeader().offset; }
    uint16_t getWindowSize() const{ return ByteOrder::hostToNetwork(getHeader().windowSize); }
    uint16_t getChecksum() const{ return ByteOrder::networkToHost(getHeader().checksum); }
    uint16_t getUrgentPointer() const{ return ByteOrder::networkToHost(getHeader().urgentPointer); }

    bool getNSFlag() const{ return getHeader().NSFlag; }
    bool getECEFlag() const{ return getHeader().ECEFlag; }
    bool getCWRFlag() const{ return getHeader().CWRFlag; }
    bool getUrgentFlag() const{ return getHeader().urgentFlag; }
    bool getAcknowledgmentFlag() const{ return getHeader().acknowledgmentFlag; }
    bool getPushFlag() const{ return getHeader().pushFlag; }
    bool getResetFlag() const{ return getHeader().resetFlag; }
    bool getSynFlag() const{ return getHeader().synFlag; }
    bool getFinishFlag() const{ return getHeader().finishFlag; }

    void setSourcePort(uint16_t sourcePort) { getHeader().sourcePort = ByteOrder::hostToNetwork(sourcePort); }
    void setDestinationPort(uint16_t destinationPort) { getHeader().destinationPort = ByteOrder::hostToNetwork(destinationPort); }
    void setSequenceNumber(uint32_t sequenceNumber) { getHeader().sequenceNumber = ByteOrder::hostToNetwork(sequenceNumber); }
    void setAcknowledgmentNumber(uint32_t acknowledgmentNumber) { getHeader().acknowledgmentNumber = ByteOrder::hostToNetwork(acknowledgmentNumber); }
    void setOffset(uint8_t offset) { getHeader().offset = offset; }
    void setWindowSize(uint16_t windowSize) { getHeader().windowSize = ByteOrder::hostToNetwork(windowSize); }
    void setChecksum(uint16_t checksum) { getHeader().checksum = ByteOrder::hostToNetwork(checksum); }
    void setUrgentPointer(uint16_t urgentPointer) { getHeader().urgentPointer = ByteOrder::hostToNetwork(urgentPointer); }

    /* Tcp Flags settings */
    void setNSFlag(bool NSFlag) { getHeader().NSFlag = NSFlag; }
    void setCWRFlag(bool CWRFlag) { getHeader().CWRFlag = CWRFlag; }
    void setECEFlag(bool ECEFlag) { getHeader().ECEFlag = ECEFlag; }
    void setUrgentFlag(bool urgentFlag) { getHeader().urgentFlag = urgentFlag; }
    void setAcknowledgmentFlag(bool acknowledgmentFlag) { getHeader().acknowledgmentFlag = acknowledgmentFlag; }
    void setPushFlag(bool pushFlag) { getHeader().pushFlag = pushFlag; }
    void setResetFlag(bool resetFlag) { getHeader().resetFlag = resetFlag; }
    void setSynFlag(bool synFlag) { getHeader().synFlag = synFlag; }
    void setFinishFlag(bool finishFlag) { getHeader().finishFlag = finishFlag; }

    bool isChecksumValid() const;

    /**
     * @brief Get the segement length: the data length + (finish || syn).
     * @return
     * @see RFC 792, page 26.
     */
    OctetVector::SizeType getSegementLength() const;

    TcpOptionsParser &getParser() { return mOptionsParser; }
    const TcpOptionsParser &getParser() const{ return mOptionsParser; }

    enum class ParsingError{
        None,
        OffsetLowerThanMinimumHeader,
        NoPlaceForOptionsInData
    };
    bool isValidPacket() const{ return mParsingError == ParsingError::None; }

    ParsingError getParsingError() const{ return mParsingError; }

    virtual void fromRawPacket(OctetVector &&rawPacket) override final;

private:
    static uint16_t calculateChecksum(const OctetVector &pseudoHeader, const OctetVector &header, const OctetVector &data);

    virtual void onPacketImport() override;
    virtual void onPacketExport() override;

    void setInvalidPacket(ParsingError parsingError);

    TcpOptionsParser mOptionsParser;
    ParsingError mParsingError = ParsingError::None;
};

} // Tcp
} // ProtocolLearn

#endif // TCPPACKET_H
