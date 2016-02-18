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

#include "NetworkFunctions.h"

namespace ProtocolLearn {

constexpr NetworkFunctions::NetworkFunctions()
{
}

uint16_t NetworkFunctions::calculateInternetChecksum(const VectorsList &vectors) {
    uint64_t result = 0;
    uint8_t temp = 0;
    bool isSecond = false;

    for(const auto &vector : vectors) {
        for(const auto &octet : vector.get()) {
            if(isSecond == false) {
                temp = octet;
                isSecond = true;
            } else {
                result += (static_cast<uint16_t>(temp) << 8) + octet;

                isSecond = false;
            }
        }
    }

    if(isSecond == true)
        result += temp;

    while(result >> 16)
        result = (result & 0xffff) + (result >> 16);

    return ~result;
}

} // namespace ProtocolLearn

