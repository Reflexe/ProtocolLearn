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

#ifndef PROTOCOLEARN_OCTETVECTOR_H
#define PROTOCOLEARN_OCTETVECTOR_H

#include <vector>
#include <list>
#include <cstdint>

#include "Debug.h"

namespace ProtocolLearn {

class OctetVector : public std::vector<uint8_t>
{
public:
    typedef uint8_t OctetType;
    typedef std::vector<uint8_t>::size_type SizeType;

    using vector::vector;
    using vector::operator =;

    static OctetVector fromArray(const uint8_t array[], SizeType arraySize);

    /**
     * @param object an object to copy from.
     */
    template<class T>
    static OctetVector fromObject(const T &object) {
        pl_assert(sizeof(T) >= sizeof(uint8_t)); // PARANOIA!

        const uint8_t *pbArray = reinterpret_cast<const uint8_t *>(&object);

        return OctetVector{pbArray, pbArray+sizeof(T)};
    }

    template<class T>
    T &getAsObject() {
        pl_assert(size() >= sizeof(T));

        return *(reinterpret_cast<T*>(data()));
    }

    template<class T>
    const T &getAsObject() const{
        pl_assert(size() >= sizeof(T));

        return *(reinterpret_cast<const T*>(data()));
    }

    template<class T>
    T toObject() const
    {
        return getAsObject<T>();
    }

    void alignTo(SizeType chunkSize, value_type value=0);

    void add(OctetVector &&octetVector);
    void add(const OctetVector &octetVector);

    std::list<OctetVector> splitTo(SizeType maximumChunkSize) const;
};

}

#endif // PROTOCOLEARN_OCTETVECTOR_H
