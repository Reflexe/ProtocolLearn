/**
 * @file byteorder.cpp
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
 * Contain a class for Big-Little Endian Convertions.
 */

#include "ByteOrder.h"

//#include <byteswap.h>

namespace ProtocolLearn {

ByteOrder::ByteOrder()
{
}







void ByteOrder::swapByteArray(uint8_t *pArray, uint32_t arrayLength) {
    uint8_t tempValue = 0;

    // Simple reverse array algorithm.
    for(uint32_t i = 0; i < arrayLength/2; ++i) {
        tempValue = pArray[arrayLength-i-1];
        pArray[arrayLength-i-1] = pArray[i];
        pArray[i] = tempValue;
    }
}

//OctetVector ByteOrder::swapOctetVector(const OctetVector &octetVector) {
//    OctetVector reverseVector;

//    for(OctetVector::size_type i = 0; i < octetVector.size(); ++i)
//    {
//        reverseVector.push_back(octetVector.at(octetVector.size()-i-1));
//    }

//    return reverseVector;
//}

} // ProtocolLearn
