/**
 * @file %{FILENAME}
 * @author shrek0 (shrek0.tk@gmail.com)
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) %YEAR% shrek0
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
#ifndef PROTOCOLLEARN_DNS_DNSPACKET_H
#define PROTOCOLLEARN_DNS_DNSPACKET_H

#include "PacketWrapper.h"

#include "BitField.h"
#include "ByteOrder.h"


namespace ProtocolLearn {
namespace Dns {

struct Question {
    uint16_t QType;
    uint16_t QClass;
}__attribute__((packed));

struct DnsHeader{
    uint16_t identifer;
    uint8_t queryOrResponseOpcodeAuthAnswerTruncRecursionDesired;
    uint8_t recursionAvailablereservedRCode;
    uint16_t entriesCount;
    uint16_t resourceRecordsCount;
    uint16_t serverResourceRecordsCount;
    uint16_t additionalRecordsCount;
};

class DnsPacket : public PacketWrapper<DnsHeader>
{
public:
    DnsPacket();

    uint16_t getIdentifer() const{ return ByteOrder::networkToHost(getHeader().identifer); }
    uint8_t getOpcode() const{ return BIT_FIELD_GET(getHeader().queryOrResponseOpcodeAuthAnswerTruncRecursionDesired, 1, 4); }
    bool getAuthAnswer() const{ return BIT_FIELD_GET(getHeader().queryOrResponseOpcodeAuthAnswerTruncRecursionDesired, 5, 1); }
    bool getTrunc() const{ return BIT_FIELD_GET(getHeader().queryOrResponseOpcodeAuthAnswerTruncRecursionDesired, 6, 1); }
    bool getRecursionDesired() const{ return BIT_FIELD_GET(getHeader().queryOrResponseOpcodeAuthAnswerTruncRecursionDesired, 7, 1); }
    bool getRecursionAvailable() const{ return BIT_FIELD_GET(getHeader().recursionAvailablereservedRCode, 0, 1); }
    uint8_t getreserved() const{ return BIT_FIELD_GET(getHeader().recursionAvailablereservedRCode, 1, 3); }
    uint8_t getRCode() const{ return BIT_FIELD_GET(getHeader().recursionAvailablereservedRCode, 4, 4); }
    uint16_t getEntriesCount() const{ return ByteOrder::networkToHost(getHeader().entriesCount); }
    uint16_t getResourceRecordsCount() const{ return ByteOrder::networkToHost(getHeader().resourceRecordsCount); }
    uint16_t getServerResourceRecordsCount() const{ return ByteOrder::networkToHost(getHeader().serverResourceRecordsCount); }
    uint16_t getAdditionalRecordsCount() const{ return ByteOrder::networkToHost(getHeader().additionalRecordsCount); }

    void setIdentifer(uint16_t identifer) { getHeader().identifer = ByteOrder::hostToNetwork(identifer); }
    void setOpcode(uint8_t opcode) { BIT_FIELD_SET(getHeader().queryOrResponseOpcodeAuthAnswerTruncRecursionDesired, 1, 4, opcode); }
    void setAuthAnswer(bool authAnswer) { BIT_FIELD_SET(getHeader().queryOrResponseOpcodeAuthAnswerTruncRecursionDesired, 5, 1, authAnswer); }
    void setTrunc(bool trunc) { BIT_FIELD_SET(getHeader().queryOrResponseOpcodeAuthAnswerTruncRecursionDesired, 6, 1, trunc); }
    void setRecursionDesired(bool recursionDesired) { BIT_FIELD_SET(getHeader().queryOrResponseOpcodeAuthAnswerTruncRecursionDesired, 7, 1, recursionDesired); }
    void setRecursionAvailable(bool recursionAvailable) { BIT_FIELD_SET(getHeader().recursionAvailablereservedRCode, 0, 1, recursionAvailable); }
    void setreserved(uint8_t reserved) { BIT_FIELD_SET(getHeader().recursionAvailablereservedRCode, 1, 3, reserved); }
    void setRCode(uint8_t rCode) { BIT_FIELD_SET(getHeader().recursionAvailablereservedRCode, 4, 4, rCode); }
    void setEntriesCount(uint16_t entriesCount) { getHeader().entriesCount = ByteOrder::hostToNetwork(entriesCount); }
    void setResourceRecordsCount(uint16_t resourceRecordsCount) { getHeader().resourceRecordsCount = ByteOrder::hostToNetwork(resourceRecordsCount); }
    void setServerResourceRecordsCount(uint16_t serverResourceRecordsCount) { getHeader().serverResourceRecordsCount = ByteOrder::hostToNetwork(serverResourceRecordsCount); }
    void setAdditionalRecordsCount(uint16_t additionalRecordsCount) { getHeader().additionalRecordsCount = ByteOrder::hostToNetwork(additionalRecordsCount); }

    bool isQuery() const{ return !BIT_FIELD_GET(getHeader().queryOrResponseOpcodeAuthAnswerTruncRecursionDesired, 0, 1); }
    bool isResponse() const{ return !isQuery(); }

    void setQueryFlag(bool query) { BIT_FIELD_SET(getHeader().queryOrResponseOpcodeAuthAnswerTruncRecursionDesired, 0, 1, !query); }
    void setResponseFlag(bool response) { setQueryFlag(!response); }
};

} // namespace Dns
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_DNS_DNSPACKET_H
