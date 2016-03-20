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

namespace ProtocolLearn {
namespace Dns {

std::pair<bool, OctetVector::SizeType> DnsDomain::fromData(const OctetVector::const_iterator &begin,
                                                           const OctetVector::const_iterator &end) {
    auto iterator = begin;

    if (iterator == end)
        return {false, 0};

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
        uint8_t labelLength = *iterator;
        ++iterator;

        if (labelLength == 0)
            break;

        if ((iterator+labelLength) > end)
            return {false, 0};

        mIteratorsList.emplace_back(iterator, iterator+labelLength);
        iterator += labelLength;
    } while (iterator < end);


    if (!mIteratorsList.empty())
        mDomainData.assign(begin, iterator);

    return {true, static_cast<OctetVector::SizeType>(iterator-begin)};
}

bool DnsDomain::fromString(const std::string &string) {
    mDomainData.assign(string.begin(), string.end());

    mDomainData.insert(mDomainData.begin(), 0);
// www.google.co.il
// \3www\6google\2co\2il

    auto labelBegin = mDomainData.begin();
    auto labelEnd = labelBegin;

    while ((labelEnd = std::find(labelEnd+1, mDomainData.end(), '.')) == mDomainData.end()) {
        *labelBegin = static_cast<uint8_t>(labelEnd-labelBegin);

        labelBegin = labelEnd;
    }

    *labelBegin = static_cast<uint8_t>(mDomainData.end()-labelBegin);
    mDomainData.push_back(0);

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

} // namespace Dns
} // namespace ProtocolLearn
