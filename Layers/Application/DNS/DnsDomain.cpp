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
#include "DnsDomain.h"

#include <algorithm>

namespace ProtocolLearn {
namespace Dns {

std::pair<bool, OctetVector::SizeType> DnsDomain::fromData(const OctetVector::const_iterator &realBegin,
                                                           const OctetVector::const_iterator &begin,
                                                           const OctetVector::const_iterator &end) {
    pl_assert(realBegin <= begin);

    if (begin == end)
        return {false, 0};

    auto iterator = begin;
    mIteratorsList.clear();

    /**
      * According to RFC 1035 4.1.2, the QNAME parameter should be a sequence of labels:
      * A length octet and then its data.
      *
      * In order to save resources, we'll copy all the domain data as it is and just
      * referer it in the iterators list which represents the begin and the end of
      * each label.
      */
    do {
        uint8_t lengthOctet = *iterator;

        if (lengthOctet == 0)
            break;

        // If this is a pointer.
        if ((lengthOctet & LabelPointerBitsSignture) == LabelPointerBitsSignture) {
            uint8_t pointer = lengthOctet & ~LabelPointerBitsSignture;

            if ((end-realBegin) <= static_cast<OctetVector::difference_type>(pointer)
                    || !addLabel(realBegin+pointer, end))
                return {false, 0};

            continue;
        }

        if (addLabel(iterator, end) == false)
            return {false, 0};
    } while (iterator < end);

    mDomainDatas = convertIteratorsListToOctetVectorList(mIteratorsList);

    return {true, static_cast<OctetVector::SizeType>(iterator-begin)};
}

bool DnsDomain::fromString(const std::string &string) {
    mIteratorsList.clear();

    mDomainDatas.assign(1, OctetVector{string.begin(), string.end()});
    auto &domainData = mDomainDatas.front();

    domainData.insert(domainData.begin(), 0);

    auto labelBegin = domainData.begin();
    auto labelEnd = labelBegin;

    while ((labelEnd = std::find(labelEnd+1, domainData.end(), '.')) == domainData.end()) {
//        *labelBegin = static_cast<uint8_t>(labelEnd-labelBegin);

        addLabel(labelBegin, labelEnd);
        labelBegin = labelEnd;
    }

    *labelBegin = static_cast<uint8_t>(domainData.end()-labelBegin);
    domainData.push_back(0);

    return true;
}

std::string DnsDomain::toString() const{
    std::string string;

    for (const auto &beginAndEnd : mIteratorsList) {
        string.insert(string.end(), beginAndEnd.first, beginAndEnd.second);
        string.push_back('.');
    }

    // Pop back the last dot.
    string.pop_back();

    return string;
}

bool DnsDomain::addLabel(const OctetVector::const_iterator &begin,
                         const OctetVector::const_iterator &end) {
    pl_assert(end > begin);

    uint8_t labelLength = *begin;

    if (labelLength >= (end-begin))
        return false;

    mIteratorsList.emplace_back(begin+1, begin+labelLength);

    return true;
}

std::deque<OctetVector>
DnsDomain::convertIteratorsListToOctetVectorList(DnsDomain::IteratorListType &iteratorsList) {
    std::deque<OctetVector> octetVectors;

    for (auto &labelBeginAndEnd : iteratorsList) {
        octetVectors.emplace_back(labelBeginAndEnd.first,
                                  labelBeginAndEnd.second);

        labelBeginAndEnd.first = octetVectors.back().begin();
        labelBeginAndEnd.second = octetVectors.back().end();
    }

    return octetVectors;
}

} // namespace Dns
} // namespace ProtocolLearn
