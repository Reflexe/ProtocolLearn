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

#include "OctetVector.h"

namespace ProtocolLearn {

OctetVector OctetVector::fromArray(const uint8_t array[], OctetVector::SizeType arraySize)
{
    return OctetVector{array, array+arraySize};
}

void OctetVector::alignTo(OctetVector::SizeType chunkSize, OctetVector::value_type value) {
    SizeType notAlignedOctets = size() % chunkSize;

    // If this is already aligned.
    if(notAlignedOctets == 0)
        return;

    SizeType requestedSize = size() + (chunkSize - notAlignedOctets);

    resize(requestedSize, value);
}

void OctetVector::add(OctetVector &&octetVector) {
    pl_assert(&octetVector != this);

    if(vector::empty())
        *this = std::move(octetVector);
    else
        return add(octetVector);
}

void OctetVector::add(const OctetVector &octetVector) {
    pl_assert(&octetVector != this);

    vector::reserve(size() + octetVector.size());
    vector::insert(end(), octetVector.begin(), octetVector.end());
}

std::list<OctetVector> OctetVector::splitTo(OctetVector::SizeType maximumChunkSize) const{
    if(size() <= maximumChunkSize)
        return std::list<OctetVector>{*this};
    if(size() == 0 || maximumChunkSize == 0)
        return std::list<OctetVector>{};

    auto requestedParts = (size()/maximumChunkSize);

    std::list<OctetVector> partsList;
    auto nextOctetToRead = cbegin();

    for(SizeType i = 0; i < requestedParts; ++i, nextOctetToRead+=maximumChunkSize)
    {
        partsList.push_back(OctetVector{nextOctetToRead, nextOctetToRead+maximumChunkSize});
    }

    if(size() % maximumChunkSize != 0)
        partsList.push_back(OctetVector{nextOctetToRead, cend()});

    return partsList;
}

}
