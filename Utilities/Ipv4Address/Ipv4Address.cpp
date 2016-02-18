#include "Ipv4Address.h"

#include <algorithm> // for std::count
#include <array>

#include <Convertions.h>
#include <StringFunctions.h>

namespace ProtocolLearn {

char Ipv4Address::ipv4AddressDelim[] = ".";

Ipv4Address::Ipv4Address()
    : mIntegerAddress{0}
{
}

Ipv4Address::Ipv4Address(const Ipv4Address::Ipv4AddressIntType &integerIpv4Address)
    : mIntegerAddress{integerIpv4Address}
{
}

Ipv4Address::Ipv4Address(const std::string &stringIpv4Address) {
    if(isValidIpv4Address(stringIpv4Address) == false)
        throw InvalidArgument("Ipv4Address:fromString");

    mIntegerAddress = ipStringtoInt(stringIpv4Address);
}

Ipv4Address::Ipv4Address(const char string[])
    : Ipv4Address(std::string(string))
{
}

Ipv4Address Ipv4Address::fromInt(const Ipv4Address::Ipv4AddressIntType &integerIpv4Address)
{
    return Ipv4Address(integerIpv4Address);
}

Ipv4Address Ipv4Address::fromString(const std::string &stringIpv4Address)
{
    return Ipv4Address(stringIpv4Address);
}

Ipv4Address Ipv4Address::fromString(const char stringIpv4Address[])
{
    return fromString(std::string(stringIpv4Address));
}

uint32_t Ipv4Address::toInt() const
{
    return mIntegerAddress;
}

std::string Ipv4Address::toString() const
{
    return ipIntToString(mIntegerAddress);
}

Ipv4Address &Ipv4Address::operator ++() {
    *this = fromInt(++mIntegerAddress);
    return *this;
}

bool Ipv4Address::inRange(const Ipv4Address &ipDestination, const Ipv4Address &ipMask) const
{
    return (toInt() & ipMask.toInt()) == ipDestination.toInt();
}

Ipv4Address &Ipv4Address::operator =(const std::string &ip) {
    *this = fromString(ip);

    return *this;
}

bool Ipv4Address::equal(const Ipv4Address &ipv4Address) const
{
    return toInt() == ipv4Address.toInt();
}

bool Ipv4Address::isBroadcastOf(const Ipv4Address &mask, const Ipv4Address &networkDestination)
{
    return toInt() == (networkDestination.toInt() | ~mask.toInt());
}

Ipv4Address &Ipv4Address::operator =(const Ipv4Address::Ipv4AddressIntType &ip) {
    *this = fromInt(ip);

    return *this;
}

bool Ipv4Address::isValidIpv4Address(const std::string &ipv4Address) {
    if(ipv4Address.size() < 7 || ipv4Address.size() > 15
            || std::count(ipv4Address.cbegin(), ipv4Address.cend(), ipv4AddressDelim[0]) != Ipv4AddressLength-1)
        return false;

    const auto &stringList = StringFunctions::split(ipv4Address, ipv4AddressDelim);

    if(stringList.size() != Ipv4AddressLength)
        return false;

    const std::string othersValidCharacters = "0123456789";

    for(const auto &str : stringList) {
        if(str.size() < 1 || str.size() > 3)
            return false;

        // Check the unity digit.
        if(othersValidCharacters.find(str.back()) == std::string::npos)
            return false;

        // Check the handerds and the tens of a three digits number.
        if(str.size() == 3) {
            const std::string hundredsDigitValidCharacters = "012";
            const std::string tensDigitValidCharacters = "012345"; // Only when the hundreds digit is '2'.

            // Check if the handreds digit is valid.
            if(hundredsDigitValidCharacters.find(str[0]) == std::string::npos)
                return false;

            // Check the tens digit of a number that its hunderds digit equal to 2.
            if(str.front() == '2' && tensDigitValidCharacters.find(str[1]) == std::string::npos)
                return false;
        }

        // Check the tens of a two or three digits number.
        if(str.size() >= 2 && othersValidCharacters.find(str[0]) == std::string::npos)
            return false;
    }

    return true;
}

/**
 * @brief Convert string ip to intger.
 * @param ipString  string to convertion.
 * @return 32 bit integer of ip address.
 * @throw InvalidArgument when @arg ipString is not a valid number.
 */
Ipv4Address::Ipv4AddressIntType Ipv4Address::ipStringtoInt(const std::string &ipString) const{
    const auto &stringList = StringFunctions::split(ipString, ipv4AddressDelim);
    std::array<uint8_t, Ipv4AddressLength> ipv4Address = {0, 0, 0, 0};
    size_t i = 0;

    for(const auto &string : stringList)
    {
        ipv4Address[i++] = Convertions::toInteger<uint8_t>(string);
    }

    uint32_t returnValue = 0;

    // Convert the byte array to a 32 bits variable.
    returnValue |= (ipv4Address[0] & 0x000000FFu) << 24;
    returnValue |= (ipv4Address[1] & 0x000000FFu) << 16;
    returnValue |= (ipv4Address[2] & 0x000000FFu) << 8;
    returnValue |= (ipv4Address[3] & 0x000000FFu);

    return returnValue;
}

std::string Ipv4Address::ipIntToString(Ipv4Address::Ipv4AddressIntType ipInt) const{
    std::array<uint8_t, Ipv4AddressLength> ipv4Address = {0, 0, 0, 0};

    // Split the Address to four bytes.
    ipv4Address[0] = (ipInt & 0xFF000000u) >> 24;
    ipv4Address[1] = (ipInt & 0x00FF0000u) >> 16;
    ipv4Address[2] = (ipInt & 0x0000FF00u) >> 8;
    ipv4Address[3] = (ipInt & 0x000000FFu);

    std::string string;

    // Convert the splited bytes into one string.
    for(const auto &ipv4AddressPart : ipv4Address)
    {
        string += Convertions::toString<uint8_t>(ipv4AddressPart) + ipv4AddressDelim;
    }

    // Remove the last dot from the end.
    string.pop_back();

    return string;
}

} // ProtocolLearn
