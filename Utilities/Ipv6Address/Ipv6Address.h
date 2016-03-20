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

#ifndef PROTOCOLLEARN_IPV6ADDRESS_H
#define PROTOCOLLEARN_IPV6ADDRESS_H

#include <cstdint>

#include <array>
#include <string>

#include "StringFunctions.h"

namespace ProtocolLearn {

class Ipv6Address // May contain bugs: not tested.
{
public:
    static const uint8_t Ipv6AddressSize = 8u;

    static const char Ipv6AddressDelim[];

    struct IntegerType{
        uint64_t first;
        uint64_t last;
    };

    Ipv6Address();

    explicit Ipv6Address(uint64_t first, uint64_t last);

    explicit Ipv6Address(const std::pair<std::array<uint8_t, 8>, std::array<uint8_t, 8>> &address);
    explicit Ipv6Address(const uint8_t address[16]);
    explicit Ipv6Address(const IntegerType &integerType);

    explicit Ipv6Address(const std::string &string);
    explicit Ipv6Address(const char string[]);

    static bool equal(const Ipv6Address &ipv6Address, const Ipv6Address &anotherIpv6Address);

    bool operator ==(const Ipv6Address &ipv6Address) const{ return equal(*this, ipv6Address); }
    bool operator !=(const Ipv6Address &ipv6Address) const{ return !equal(*this, ipv6Address); }

    const IntegerType &toInteger() const{ return mIntegerType; }

private:
    static std::pair<std::array<uint8_t, 8>, std::array<uint8_t, 8>> split16OctetArrayTo8(const uint8_t address[16]);
    static uint64_t arrayToInteger(const std::array<uint8_t, 8> &array);

    static uint64_t convertHalfAddressToInteger(const std::list<std::string> &stringList);

    static IntegerType stringToInteger(const std::string &string);
    static std::string integerToString(const IntegerType &integerType);

    static bool isValidIpv6Address(const std::list<std::string> &ipv6Parts);

    IntegerType mIntegerType;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_IPV6ADDRESS_H
