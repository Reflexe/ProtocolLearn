/**
 * @file vectorfunctions.h
 * @author shrek0 (shrek0.tk@gmail.com)
 * @class VectorFunctions
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

#ifndef VECTORFUNCTIONS_H
#define VECTORFUNCTIONS_H

#include <vector>
#include <cstdint>
#include <cstring>

#include "Debug.h"

namespace ProtocolLearn {

class VectorFunctions
{
public:
    constexpr VectorFunctions();

    /**
     * @param pArray  array to copy from.
     * @param arrayLength  pArray's length.
     * @param vector  vector to copy to.
     */
    template<class T>
    static void copyArrayToVector(const T *pArray, size_t arrayLength, std::vector<uint8_t> &vector) {
        pl_assert(sizeof(T) >= sizeof(uint8_t));

        const uint8_t *pbArray = reinterpret_cast<const uint8_t *>(pArray);

        vector.assign(pbArray, pbArray+(arrayLength*sizeof(T)));
    }

//    /**
//     * @param vector  vector to copy from.
//     * @param pArray  array to copy to.
//     * @param maxSize  array's max size (in elements).
//     */
//    template<class T>
//    static void copyVectorToArray(const std::vector<uint8_t> &vector, T *pArray, size_t maxSize) {
//        // If maxSize is grather than vector.size(), copy vector.size() elements.
//        // Else, copy maxSize elements.
//        pl_assert(vector.size() <= maxSize);

//        std::copy(vector.cbegin(), vector.cend(), pArray);
//    }

    /**
     * @param pArray  array to copy from.
     * @param arrayLength  pArray's length.
     */
    template<class T>
    static std::vector<uint8_t> arrayToVector(const T *pArray, size_t arrayLength) {
        std::vector<uint8_t> vector;

        copyArrayToVector(pArray, arrayLength, vector);

        return vector;
    }

    template<class T>
    static T vectorToObject(const std::vector<uint8_t> &vector) {
        pl_assert(vector.size() >= sizeof(T));

        return *(reinterpret_cast<const T*>(vector.data()));
    }
};

} // ProtocolLearn

#endif // VECTORFUNCTIONS_H
