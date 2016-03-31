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
#include "InternetChecksum.h"

namespace ProtocolLearn {

InternetChecksum::InternetChecksum()
{

}

void InternetChecksum::add(const uint8_t octet)  {
    if (!mIsSecond) {
        mTemp = octet;
        mIsSecond = true;
    } else {
        mResult += static_cast<uint32_t>((static_cast<uint16_t>(mTemp) << 8) + static_cast<uint16_t>(octet));

        mIsSecond = false;
    }
}

void InternetChecksum::add(const OctetVector &octetVector) {
    for(const uint8_t &octet : octetVector)
    {
        add(octet);
    }
}

uint16_t InternetChecksum::calculateInternetChecksum() {
    if(mIsSecond == true)
        mResult += mTemp;

    while(mResult >> 16)
        mResult = (mResult & 0xffff) + (mResult >> 16);

    return static_cast<uint16_t>(~mResult);
}

} // namespace ProtocolLearn
