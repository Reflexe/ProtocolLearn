#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <string>
#include <array>

#include <cstring>

#include "StringFunctions.h"
#include "Convertions.h"

namespace ProtocolLearn {

class MacAddress
{
public:
    // Flags.
    static const uint8_t macFlagMulticast  = 0b10000000;
    static const uint8_t macFlagUnicast    = 0b00000000;

    static const char macAddressDelim[];

    static const std::size_t MacAddressLength = 6;

    typedef std::array<uint8_t, MacAddressLength> MacIntegerType;

    /**
     * @brief BroadcastAddress  Contains the ethernet broadcast address (ff:ff:ff:ff:ff:ff)
     */
    static const MacAddress BroadcastAddress;

    /**
     * @brief MacAddress  Default constructor: sets the address to 00:00:00:00:00:00.
     */
    MacAddress();

    /**
     * @brief MacAddress  Initialize the address by a numeric address.
     * @param numericAddress
     */
    explicit MacAddress(const MacIntegerType &numericAddress);

    /**
     * @brief MacAddress  Initialize the address by a null-terminated string.
     * @param string
     */
    explicit MacAddress(const char *string);

    /**
     * @brief MacAddress  Initialize the address by a std::string.
     * @param stringAddress
     */
    explicit MacAddress(const std::string &stringAddress);

    ~MacAddress();

    MacAddress &operator = (const MacIntegerType &numericAddress);

    MacAddress &operator = (const std::string &stringAddress);

    MacAddress &operator = (const uint8_t numericAddress[MacAddressLength]);

    MacAddress &operator = (const char *string);

    bool operator == (const MacAddress &macAddress) const;
    bool operator != (const MacAddress &macAddress) const;

    bool isUnicast() const;

    bool isMulticast() const;

    void setMulticast();

    void setUnicast();

    static MacAddress fromString(const std::string &address);

    static MacAddress fromString(const char *string);

    static MacAddress fromNumeric(const uint8_t address[]);

    static MacAddress fromNumeric(const MacIntegerType &address);

    static MacAddress fromNumeric(const uint64_t *address);

    void copyNumeric(uint8_t *address) const;

    const std::string &toString() const;

    const uint8_t *toByteArrayNumeric() const;

    const MacIntegerType &toNumeric() const;

    uint64_t to64BitNumeric() const;

    static bool isValidMacAddrsss(const std::string &macAddress);

private:
    static std::string macIntegerToString(const MacIntegerType &integerAddress);
    static MacIntegerType macStringToInteger(const std::string &stringAddress);

    std::array<uint8_t, MacAddressLength> mNumericAddress;
    std::string mStringAddress;
};

} // ProtocolLearn

#endif // MACADDRESS_H
