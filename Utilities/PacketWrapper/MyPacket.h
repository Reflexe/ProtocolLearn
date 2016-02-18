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

#ifndef MYPACKET_H
#define MYPACKET_H

#include <cstdint>

#include "ByteOrder.h"
#include "PacketWrapper.h"

namespace ProtocolLearn {


class MyPacketAddress
{
public:
    void from32BitInteger(uint32_t integer)
    {
        this->integer = integer;
    }

    uint32_t to32BitInteger() const{ return integer; }

private:
    uint32_t integer = 0;
};

struct MyPacketHeader{
    uint8_t field;
    uint16_t anotherField;
    uint64_t bigField;
    OctetVector::SizeType packetLength;
    OctetVector::SizeType headerLength;
    OctetVector::SizeType dataLength;
    uint32_t address;
}__attribute__((packed));

class MyPacket : public PacketWrapper<MyPacketHeader>
{
public:
    MyPacket();

    uint8_t getField() const{ return getHeader().field; }
    uint16_t getAnotherField() const{ return ByteOrder::networkToHost(getHeader().anotherField); }
    uint64_t getBigField() const{ return ByteOrder::networkToHost(getHeader().bigField); }
    uint32_t getAddress() const { return miniPacket.address.to32BitInteger(); }
    OctetVector::SizeType getPacketLengthField() { return ByteOrder::networkToHost(getHeader().packetLength); }

    void setField(uint8_t field) { getHeader().field = field; }
    void setAnotherField(uint16_t anotherField) { getHeader().anotherField = ByteOrder::hostToNetwork(anotherField); }
    void setBigField(uint64_t bigField) { getHeader().bigField = ByteOrder::hostToNetwork(bigField); }
    void setAddress(uint32_t address) { miniPacket.address.from32BitInteger(address); }

    virtual void onPacketImport() override final;
    virtual void onPacketExport() override final;

    struct {
        MyPacketAddress address;
    } miniPacket;
};

}

#endif // MYPACKET_H
