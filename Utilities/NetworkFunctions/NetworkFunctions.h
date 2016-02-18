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

#ifndef PROTOCOLLEARN_NETWORKFUNCTIONS_H
#define PROTOCOLLEARN_NETWORKFUNCTIONS_H

#include <functional>
#include <list>
#include <cstdint>

#include "OctetVector.h"

namespace ProtocolLearn {

class NetworkFunctions
{
public:
    constexpr NetworkFunctions();

    typedef std::reference_wrapper<const OctetVector> VectorReference;
    typedef std::list<VectorReference> VectorsList;

    /**
     * @brief calculateInternetChecksum  Calc the internet checksum as explained in RFC1071.
     * @param vectors
     * @return The checksum in the host byte order.
     */
    static uint16_t calculateInternetChecksum(const VectorsList &vectors);
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_NETWORKFUNCTIONS_H
