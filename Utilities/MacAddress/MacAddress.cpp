#include "MacAddress.h"

#include <algorithm>

namespace ProtocolLearn {

const char MacAddress::macAddressDelim[] = ":";
const MacAddress MacAddress::BroadcastAddress("ff:ff:ff:ff:ff:ff");

MacAddress::MacAddress()
    : mNumericAddress{{0, 0, 0, 0, 0, 0}}, mStringAddress("00:00:00:00:00:00")
{
}

MacAddress::MacAddress(const MacAddress::MacIntegerType &numericAddress)
    :  mNumericAddress(numericAddress), mStringAddress(macIntegerToString(numericAddress))
{
}

MacAddress::MacAddress(const char *string)
    : MacAddress(std::string(string))
{
}

MacAddress::MacAddress(const std::string &stringAddress)
    : mStringAddress(stringAddress) {

    if(isValidMacAddrsss(stringAddress) == false)
        throw InvalidArgument("MacAddress::fromString");

    mNumericAddress = macStringToInteger(stringAddress);
}

MacAddress &MacAddress::operator =(const uint8_t numericAddress[MacAddressLength]) {
    *this = fromNumeric(numericAddress);

    return *this;
}

MacAddress &MacAddress::operator =(const char *string) {
    *this = fromString(string);

    return *this;
}

MacAddress &MacAddress::operator =(const MacAddress::MacIntegerType &numericAddress) {
    *this = fromNumeric(numericAddress);

    return *this;
}

bool MacAddress::operator ==(const MacAddress &macAddress) const
{
    return toNumeric() == macAddress.toNumeric();
}

bool MacAddress::operator !=(const MacAddress &macAddress) const
{
    return macAddress.to64BitNumeric() != to64BitNumeric();
}

bool MacAddress::isUnicast() const
{
    return !isMulticast();
}

bool MacAddress::isMulticast() const
{
    return (mNumericAddress[0] & macFlagMulticast) == macFlagMulticast;
}

void MacAddress::setMulticast() {
    if(isUnicast())
        mNumericAddress[0] |= macFlagUnicast;
}

void MacAddress::setUnicast() {
    if(isMulticast())
        mNumericAddress[0] &= 127;
}

MacAddress MacAddress::fromString(const std::string &address)
{
    return MacAddress(address);
}

MacAddress MacAddress::fromString(const char *string)
{
    return fromString(std::string(string));
}

MacAddress MacAddress::fromNumeric(const uint8_t address[MacAddressLength]) {
    MacIntegerType integerAddress;

    for(MacIntegerType::size_type i = 0; i < MacAddressLength; ++i)
    {
        integerAddress[i] = address[i];
    }

    return fromNumeric(integerAddress);
}

MacAddress MacAddress::fromNumeric(const MacAddress::MacIntegerType &address)
{
    return MacAddress(address);
}

MacAddress MacAddress::fromNumeric(const uint64_t *address)
{
    return fromNumeric(reinterpret_cast<const uint8_t*>(address));
}

void MacAddress::copyNumeric(uint8_t *address) const{
    for(size_t i = 0; i < MacAddressLength; ++i)
    {
        address[i] = mNumericAddress[i];
    }
}

const std::string &MacAddress::toString() const
{
    return mStringAddress;
}

const uint8_t *MacAddress::toByteArrayNumeric() const
{
    return mNumericAddress.data();
}

const MacAddress::MacIntegerType &MacAddress::toNumeric() const
{
    return mNumericAddress;
}

uint64_t MacAddress::to64BitNumeric() const{
    uint64_t returnValue = 0;

    copyNumeric(reinterpret_cast<uint8_t *>(&returnValue));

    return returnValue;
}

bool MacAddress::isValidMacAddrsss(const std::string &macAddress) {
    // Check the all string size to be strlen("ff:ff:ff:ff:ff:ff") or strlen("f:f:f:f:f:f").
    if(macAddress.size() > 17 || macAddress.size() < 11
            || std::count(macAddress.cbegin(), macAddress.cend(), macAddressDelim[0]) != MacAddressLength-1)
        return false;

    const auto &stringList = StringFunctions::split(macAddress, macAddressDelim);

    // Check the times that ':' repeats.
    if(stringList.size() != MacAddressLength)
        return false;

    const std::string validCharacters = "0123456789abcdefABCDEF";

    for(const auto &str : stringList) {
        if(str.size() != 2 && str.size() != 1)
            return false;

        // Check if str[0-1] is in the validCharacters.
        if(validCharacters.find(str[0]) == std::string::npos
                || (str.size() == 2 && validCharacters.find(str[1]) == std::string::npos))
            return false;
    }

    return true;
}

std::string MacAddress::macIntegerToString(const MacIntegerType &integerAddress) {
    std::string stringAddress = "";

    for(const auto &integer : integerAddress)
    {
        stringAddress += Convertions::toString<uint8_t>(integer, 16) + macAddressDelim;
    }

    // Remove the last ":".
    stringAddress.pop_back();

    return stringAddress;
}

MacAddress::MacIntegerType MacAddress::macStringToInteger(const std::string &stringAddress) {
    auto stringList = StringFunctions::split(stringAddress, macAddressDelim);
    MacIntegerType integerAddress;
    MacIntegerType::size_type i = 0;

    for(const auto &string : stringList)
    {
        integerAddress[i++] = Convertions::toInteger<uint8_t>(string, 16);
    }

    return integerAddress;
}

} // ProtocolLearn
