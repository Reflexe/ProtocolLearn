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

#ifndef PROTOCOLLEARN_BITFIELD_H
#define PROTOCOLLEARN_BITFIELD_H

#include <limits>
#include <cstdint>
#include <type_traits>

namespace ProtocolLearn {

#define BIT_FIELD_SET(var, begin, length, value) ((var) = BitField<decltype(var)>::setPositionAndReturn<begin, length>((var), (value)))
#define BIT_FIELD_GET(var, begin, length) (BitField<decltype(var)>::getPosition<begin, length>((var)))

#define BIT_FIELD_GET_NET(var, begin, length) (BIT_FIELD_GET(ByteOrder::networkToHost((var)) (begin), (length)))
#define BIT_FIELD_SET_NET(var, begin, length, value) ((ByteOrder::hostToNetwork((var))\
    = BitField<decltype(var)>::setPositionAndReturn<begin, length>(ByteOrder::networkToHost((var)), (value))))

template<typename _Type>
struct BitField {
    typedef _Type FieldType;

    /**
     * @arg begin  from the MSB.
     * @arg end    from the MSB, not including.
     */
    template<uint16_t begin, uint16_t end>
    struct BitRange {
        typedef std::numeric_limits<FieldType> Limits;

        static_assert(std::is_unsigned<FieldType>::value == true, "Your _Type isn't unsigned");

        static_assert(end > begin, "Invalid end or begin.");
        static_assert(end <= Limits::digits, "Invalid end");

        static constexpr FieldType MaxValue = Limits::max();
        static constexpr uint16_t OffsetFromLSB = Limits::digits - end; /* The bit offset of the mask's LSB from the integer's LSB */
        static constexpr FieldType FieldMask = (MaxValue << OffsetFromLSB) & (MaxValue >> begin);
    };

    template<uint16_t begin, uint16_t length, typename _BitRangeType=BitRange<begin, begin+length>>
    static void setPosition(FieldType &previousValue, FieldType value)
    {
        previousValue = ( ((value << _BitRangeType::OffsetFromLSB) & _BitRangeType::FieldMask) | (previousValue & (~_BitRangeType::FieldMask)) );
    }

    template<uint16_t begin, uint16_t length, typename _BitRangeType=BitRange<begin, begin+length>>
    static FieldType setPositionAndReturn(FieldType previousValue, FieldType value) {
        FieldType returnValue = previousValue;

        setPosition<begin, length, _BitRangeType>(returnValue, value);

        return returnValue;
    }

    template<uint16_t begin, uint16_t length, typename _BitRangeType=BitRange<begin, begin+length>>
    static FieldType getPosition(FieldType previousValue)
    {   // Get the required bits and move them to the start.
        return (previousValue & _BitRangeType::FieldMask) >> _BitRangeType::OffsetFromLSB;
    }
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_BITFIELD_H
