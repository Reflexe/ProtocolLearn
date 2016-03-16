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
#ifndef PROTOCOLLEARN_INTERNETCHECKSUM_H
#define PROTOCOLLEARN_INTERNETCHECKSUM_H

#include <cstdint>

#include "OctetVector.h"

namespace ProtocolLearn {

class InternetChecksum
{
public:
    InternetChecksum();

    void add(const uint8_t octet);

    void add(const OctetVector &octetVector);

    /**
     * @brief calculateInternetChecksum  Calc the internet checksum as explained in RFC1071.
     * @param vectors
     * @return The checksum in the host byte order.
     */
    uint16_t calculateInternetChecksum();

private:
    uint64_t mResult = 0;
    uint8_t mTemp = 0;
    bool mIsSecond = false;

};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_INTERNETCHECKSUM_H
