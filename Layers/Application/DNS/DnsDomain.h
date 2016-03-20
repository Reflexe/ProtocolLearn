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
#ifndef PROTOCOLLEARN_DNS_DNSDOMAIN_H
#define PROTOCOLLEARN_DNS_DNSDOMAIN_H

#include <string>
#include <vector>

#include "OctetVector.h"

namespace ProtocolLearn {
namespace Dns {

// A list of dns labels.
class DnsDomain
{
public:
    DnsDomain();

    std::pair<bool, OctetVector::SizeType> fromData(const OctetVector::const_iterator &begin, const OctetVector::const_iterator &end);
    bool fromString(const std::string &string);

    std::string toString() const;

private:
    typedef std::vector<std::pair<OctetVector::const_iterator, OctetVector::const_iterator>> IteratorListType;

    OctetVector mDomainData;
    IteratorListType mIteratorsList;
};

} // namespace Dns
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_DNS_DNSDOMAIN_H
