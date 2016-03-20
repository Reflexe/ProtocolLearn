/**
 * @file rtattrparser.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
  RtattrParser
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

#include "RtAttrParser.h"

namespace ProtocolLearn {

RtattrParser::RtattrParser()
{ }

void RtattrParser::parse(OctetVector::const_iterator begin, OctetVector::const_iterator end) {
    const rtattr *prtattr = reinterpret_cast<const rtattr*>(&(*begin));
    OctetVector::SizeType dataToParseLength = static_cast<OctetVector::SizeType>(end-begin);
    const OctetVector::size_type dataLength = dataToParseLength;

    while(RTA_OK(prtattr, dataToParseLength)) {
        // If rta_len + the data we've parsed is greather than dataLength. It's assert because we're talking to the Linux kernel.
        pl_assert(prtattr->rta_len + (dataLength - dataToParseLength) <= dataLength);

        setOption(prtattr->rta_type, myrtattr{OctetVector::fromArray(static_cast<const uint8_t*>(RTA_DATA(prtattr)),
                                              prtattr->rta_len-sizeof(*prtattr))});

        prtattr = RTA_NEXT(prtattr, dataToParseLength);
    }

    pl_assert(dataToParseLength == 0);
}

OctetVector RtattrParser::toOctetVector() const{
    OctetVector data;
    rtattr attr;
    OctetVector attributeTempData;

    for(const auto &attribute : mOptionsMap) {
        attr.rta_type = attribute.first;
        attr.rta_len = static_cast<unsigned short>(RTA_LENGTH(attribute.second.data.size()));

        attributeTempData = OctetVector::fromObject(attr);

        // Add the attribute and the attribute's data to the vector.
        data.add(attributeTempData);
        data.add(attribute.second.data);
    }

    return data;
}

} // ProtocolLearn
