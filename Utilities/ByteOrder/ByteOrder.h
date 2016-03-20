/**
 * @file byteorder.h
 * @author shrek0 (shrek0.tk@gmail.com)
  ByteOrder
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
 * Contains a class for Big-Little Endian Convertions.
 */

#ifndef BYTEORDER_H
#define BYTEORDER_H

#include <cstdint>

#include "OctetVector.h"

#include <limits>

namespace ProtocolLearn {

class ByteOrder
{
public:
    ByteOrder();

    /* Main Little-Big Endian Convertion Functions. */

    constexpr static uint16_t swap16(uint16_t value) {
        //    uint16_t result = 0;

        //    result |= (value & 0x00FF) << 8;
        //    result |= (value & 0xFF00) >> 8;

        return static_cast<uint16_t>((((value & 0x00FF) << 8)
                | ((value & 0xFF00) >> 8)));

        //    return bswap_16(value);
    }

    constexpr static uint32_t swap32(uint32_t value) {
        //    uint32_t result = 0;

        //    result |= (value & 0x000000FF) << 24;
        //    result |= (value & 0x0000FF00) << 8;
        //    result |= (value & 0x00FF0000) >> 8;
        //    result |= (value & 0xFF000000) >> 24;

        return ((value & 0x000000FF) << 24)
                | ((value & 0x0000FF00) << 8)
                | ((value & 0x00FF0000) >> 8)
                | ((value & 0xFF000000) >> 24);

        //    return bswap_32(value);
    }

    constexpr static uint64_t swap64(uint64_t value) {
        //    uint64_t result = 0;

        //    result |= (value & 0x00000000000000FF) << 56;
        //    result |= (value & 0x000000000000FF00) << 40;
        //    result |= (value & 0x0000000000FF0000) << 24;
        //    result |= (value & 0x00000000FF000000) << 8;
        //    result |= (value & 0x000000FF00000000) >> 8;
        //    result |= (value & 0x0000FF0000000000) >> 24;
        //    result |= (value & 0x00FF000000000000) >> 40;
        //    result |= (value & 0xFF00000000000000) >> 56;

        //    return result;

        return ((value & 0x00000000000000FF) << 56)
                | ((value & 0x000000000000FF00) << 40)
                | ((value & 0x0000000000FF0000) << 24)
                | ((value & 0x00000000FF000000) << 8)
                | ((value & 0x000000FF00000000) >> 8)
                | ((value & 0x0000FF0000000000) >> 24)
                | ((value & 0x00FF000000000000) >> 40)
                | ((value & 0xFF00000000000000) >> 56);

        //    return bswap_64(value);
    }

    static void swapByteArray(uint8_t *pArray, uint32_t arrayLength);

    template<typename Container>
    static Container swapContainer(const Container &container)
    {   // Finally I've found a use for Reverse Iterators.
        return Container{container.rbegin(), container.rend()};
    }

#if BYTE_ORDER == LITTLE_ENDIAN
    /* Little Endian Convert Functions */

    // 16 Bits.
    static constexpr uint16_t littleEndianToHost(uint16_t value)
    {
        return value;
    }

    static constexpr uint16_t bigEndianToHost(uint16_t value)
    {
        return swap16(value);
    }

    static constexpr uint16_t hostToLittleEndian(uint16_t value)
    {
        return value;
    }

    static constexpr uint16_t hostToBigEndian(uint16_t value)
    {
        return swap16(value);
    }

    // 32 Bits.
    static constexpr  uint32_t littleEndianToHost(uint32_t value)
    {
        return value;
    }

    static constexpr uint32_t bigEndianToHost(uint32_t value)
    {
        return swap32(value);
    }

    static constexpr uint32_t hostToLittleEndian(uint32_t value)
    {
        return value;
    }

    static constexpr uint32_t hostToBigEndian(uint32_t value)
    {
        return swap32(value);
    }

    // 64 Bits.
    static constexpr uint64_t littleEndianToHost(uint64_t value)
    {
        return value;
    }

    static constexpr uint64_t bigEndianToHost(uint64_t value)
    {
        return swap64(value);
    }

    static constexpr uint64_t hostToLittleEndian(uint64_t value)
    {
        return value;
    }

    static constexpr uint64_t hostToBigEndian(uint64_t value)
    {
        return swap64(value);
    }

    // Byte Array.
    static void littleEndianToHost(uint8_t *, uint32_t)
    {
    }

    static void bigEndianToHost(uint8_t *pArray, uint32_t arrayLength)
    {
        return swapByteArray(pArray, arrayLength);
    }

    static void hostToLittleEndian(uint8_t *, uint32_t)
    {
    }

    static void hostToBigEndian(uint8_t *pArray, uint32_t arrayLength)
    {
        return swapByteArray(pArray, arrayLength);
    }

    // Byte Vector
    static OctetVector littleEndianToHost(const OctetVector &vector)
    {
        return OctetVector(vector); // Create new vector.
    }

    static OctetVector bigEndianToHost(const OctetVector &vector)
    {
        return swapContainer(vector);
    }

    static OctetVector hostToLittleEndian(const OctetVector &vector)
    {
        return OctetVector(vector);
    }

    static OctetVector hostToBigEndian(const OctetVector &vector)
    {
        return swapContainer(vector);
    }
#else
    /* Big Endian Convert Functions */

    // 16 Bits.
    static constexpr uint16_t littleEndianToHost(uint16_t value)
    {
        return swap16(value);
    }

    static constexpr uint16_t bigEndianToHost(uint16_t value)
    {
        return value;
    }

    static constexpr uint16_t hostToLittleEndian(uint16_t value)
    {
        return swap16(value);
    }

    static constexpr uint16_t hostToBigEndian(uint16_t value)
    {
        return value;
    }

    // 32 Bits.
    static constexpr uint32_t littleEndianToHost(uint32_t value)
    {
        return swap32(value);
    }

    static constexpr uint32_t bigEndianToHost(uint32_t value)
    {
        return value;
    }

    static constexpr uint32_t hostToLittleEndian(uint32_t value)
    {
        return swap32(value);
    }

    static constexpr uint32_t hostToBigEndian(uint32_t value)
    {
        return value;
    }

    // 64 Bits.
    static constexpr uint64_t littleEndianToHost(uint64_t value)
    {
        return swap64(value);
    }

    static constexpr uint64_t bigEndianToHost(uint64_t value)
    {
        return value;
    }

    static constexpr uint64_t hostToLittleEndian(uint64_t value)
    {
        return swap64(value);
    }

    static constexpr uint64_t hostToBigEndian(uint64_t value)
    {
        return value;
    }

    // Byte Array.
    static void littleEndianToHost(uint8_t *pArray, size_t arrayLength)
    {
        return swapByteArray(pArray, arrayLength);
    }

    static void bigEndianToHost(uint8_t *, size_t)
    {
    }

    static void hostToLittleEndian(uint8_t *pArray, size_t arrayLength)
    {
        return swapByteArray(pArray, arrayLength);
    }

    static void hostToBigEndian(uint8_t *, size_t)
    {
    }

    // Byte Vector
    static OctetVector littleEndianToHost(const OctetVector &vector)
    {
        return swapContainer(vector);
    }

    static OctetVector bigEndianToHost(const OctetVector &vector)
    {
        return OctetVector(vector); // Create new vector.
    }

    static OctetVector hostToLittleEndian(const OctetVector &vector)
    {
        return swapContainer(vector);
    }

    static OctetVector hostToBigEndian(const OctetVector &vector)
    {
        return OctetVector(vector);
    }
#endif
    /* Host-Network Convert Functions */

    // Network to Host
    static constexpr uint16_t hostToNetwork(uint16_t value)
    {
        return hostToBigEndian(value);
    }

    static constexpr uint32_t hostToNetwork(uint32_t value)
    {
        return hostToBigEndian(value);
    }

    static constexpr uint64_t hostToNetwork(uint64_t value)
    {
       return hostToBigEndian(value);
    }

    static void hostToNetwork(uint8_t *pArray, uint32_t arrayLength)
    {
        return hostToBigEndian(pArray, arrayLength);
    }

    static OctetVector hostToNetwork(const OctetVector &vector)
    {
        return hostToBigEndian(vector);
    }

    // Host to Network
    static constexpr uint16_t networkToHost(uint16_t value)
    {
        return bigEndianToHost(value);
    }

    static constexpr uint32_t networkToHost(uint32_t value)
    {
        return bigEndianToHost(value);
    }

    static constexpr uint64_t networkToHost(uint64_t value)
    {
        return bigEndianToHost(value);
    }

    static void networkToHost(uint8_t *pArray, uint32_t arrayLength)
    {
        return bigEndianToHost(pArray, arrayLength);
    }

    static OctetVector networkToHost(const OctetVector &vector)
    {
        return bigEndianToHost(vector);
    }

    /**
     * Just for the compatibility with the original libary functions.
     */
    static constexpr uint16_t hton(uint16_t value)
    {
        return hostToNetwork(value);
    }

    static constexpr uint32_t hton(uint32_t value)
    {
        return hostToBigEndian(value);
    }

    static constexpr uint64_t hton(uint64_t value)
    {
        return hostToNetwork(value);
    }

    static constexpr uint16_t ntoh(uint16_t value)
    {
        return networkToHost(value);
    }

    static constexpr uint32_t ntoh(uint32_t value)
    {
        return networkToHost(value);
    }

    static constexpr uint64_t ntoh(uint64_t value)
    {
        return networkToHost(value);
    }
};

static_assert(ByteOrder::swap16(0xff00) == 0x00ff, "Fix: ByteOrder");

} // ProtocolLearn

#endif // BYTEORDER_H
