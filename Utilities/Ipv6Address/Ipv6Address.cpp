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

#include "Ipv6Address.h"

#include "Convertions.h"

namespace ProtocolLearn {

const char Ipv6Address::Ipv6AddressDelim[] = "::";

Ipv6Address::Ipv6Address()
    : mIntegerType{0, 0}
{
}

Ipv6Address::Ipv6Address(uint64_t first, uint64_t last)
    : mIntegerType{first, last}
{
}

Ipv6Address::Ipv6Address(const std::pair<std::array<uint8_t, 8>, std::array<uint8_t, 8>> &address)
    : mIntegerType{ arrayToInteger(address.first), arrayToInteger(address.second) }
{
}

Ipv6Address::Ipv6Address(const uint8_t address[16])
    : Ipv6Address{split16OctetArrayTo8(address)}
{
}

Ipv6Address::Ipv6Address(const Ipv6Address::IntegerType &integerType)
    : mIntegerType{integerType}
{
}

Ipv6Address::Ipv6Address(const std::string &string)
    : mIntegerType{stringToInteger(string)}
{
}

Ipv6Address::Ipv6Address(const char string[])
    : Ipv6Address{std::string{string}}
{
}

bool Ipv6Address::equal(const Ipv6Address &ipv6Address, const Ipv6Address &anotherIpv6Address) {
    const auto &integer = ipv6Address.toInteger();
    const auto &anotherInteger = anotherIpv6Address.toInteger();

    return integer.first == anotherInteger.first && integer.last == anotherInteger.last;
}

std::pair<std::array<uint8_t, 8>, std::array<uint8_t, 8>> Ipv6Address::split16OctetArrayTo8(const uint8_t address[16]) {
    std::array<uint8_t, 8> first;
    std::array<uint8_t, 8> last;

    std::copy(address, address+8, first.begin());
    std::copy(address+8, address+16, last.begin());

    return std::make_pair(first, last);
}

uint64_t Ipv6Address::arrayToInteger(const std::array<uint8_t, 8> &array) {
    uint64_t retValue = 0;
    uint8_t i = 64-8;

    for(const auto &octet : array) {
        retValue |= (static_cast<uint64_t>(octet) << i);

        i-=8;
    }

    return retValue;
}

uint64_t Ipv6Address::convertHalfAddressToInteger(const std::list<std::string> &stringList) {
    std::string temp;

    for(const auto &string : stringList) {
        if(string.size() != 4)
            temp.insert(0, 4-string.size(), '0');

        temp += string;
    }

    return Convertions::toInteger<uint64_t>(temp, 16);
}

Ipv6Address::IntegerType Ipv6Address::stringToInteger(const std::string &string) {
    auto splitedString = StringFunctions::split(string, std::string(Ipv6AddressDelim));
    if(isValidIpv6Address(splitedString) == false)
        throw InvalidArgument{"Invalid string format"};

    std::list<std::string> stringList;

    {
        auto it = stringList.begin();
        std::advance(it, 3);

        splitedString.splice(splitedString.begin(), stringList, stringList.begin(), it);
    }

    IntegerType integer;

    integer.first = convertHalfAddressToInteger(splitedString);
    integer.last = convertHalfAddressToInteger(stringList);

    return integer;
}

std::string Ipv6Address::integerToString(const Ipv6Address::IntegerType &integerType) {
    std::string returnValue = Convertions::toString<uint64_t>(integerType.first, 16)
                              + Convertions::toString<uint64_t>(integerType.last, 16);

    for(uint i = 0; i < Ipv6AddressSize; ++i)
    {
        returnValue.insert(i*4, Ipv6AddressDelim);
    }

    return returnValue;
}

bool Ipv6Address::isValidIpv6Address(const std::list<std::string> &ipv6Parts) {
    if(ipv6Parts.size() != Ipv6AddressSize)
        return false;

    std::string ipv6ValidCharacters = "abcdefABCDEF0123456789";

    for(const auto &part : ipv6Parts) {
        if(part.size() > 4 || part.size() == 0)
            return false;

        for(const auto &character : part) {
            if(ipv6ValidCharacters.find(character) == std::string::npos)
                return false;
        }
    }

    return true;
}

} // namespace ProtocolLearn

