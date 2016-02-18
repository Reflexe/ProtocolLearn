#ifndef IPV4ADDRESS_H
#define IPV4ADDRESS_H

#include <string>
#include <vector>

namespace ProtocolLearn {

/**
 * @brief The Ipv4Address class  An Ipv4 Address.
 */
class Ipv4Address
{
public:
    typedef uint32_t Ipv4AddressIntType;
    static const uint8_t Ipv4AddressLength = 4;
    static char ipv4AddressDelim[];

    /**
     * @brief Ipv4Address  Init the address to '0.0.0.0'.
     */
    Ipv4Address();

    /**
     * @brief Ipv4Address
     * @param integerIpv4Address in the host Byte Order.
     */
    explicit Ipv4Address(const Ipv4AddressIntType &integerIpv4Address);

    /**
     * @brief Ipv4Address
     * @param stringIpv4Address
     * @throw InvalidArgument @arg stringIpv4Address isn't a valid ipv4address.
     */
    explicit Ipv4Address(const std::string &stringIpv4Address);

    /**
     * @brief Ipv4Address
     * @param string
     * @throw InvalidArgument @arg string isn't a valid ipv4address.
     */
    explicit Ipv4Address(const char string[]);

    /**
     * @brief toInt  Get the Address as a 32-Bit integer (In the host's byte order)
     * @return
     */
    uint32_t toInt() const;

    /**
     * @brief toString  Get the address as a std::string in n.n.n.n format.
     * @return
     */
    std::string toString() const;

    static Ipv4Address fromInt(const Ipv4AddressIntType &integerIpv4Address);

    static Ipv4Address fromString(const std::string &stringIpv4Address);

    static Ipv4Address fromString(const char stringIpv4Address[]);

    Ipv4Address &operator = (const Ipv4AddressIntType &ip);
    Ipv4Address &operator = (const std::string &ip);

    bool equal(const Ipv4Address &ipv4Address) const;

    bool operator == (const Ipv4Address &ipv4Address) const{ return equal(ipv4Address); }
    bool operator != (const Ipv4Address &ipv4Address) const{ return !equal(ipv4Address); }
    bool operator == (const uint32_t &ipv4Address) const{ return ipv4Address == toInt(); }

    /**
     * @brief isBroadcast  Checks if the address is 255.255.255.255.
     * @return ipv4Address == 255.255.255.255.
     */
    bool isBroadcast() { return *this == Ipv4Address::fromString("255.255.255.255"); }

    /**
     * @brief isBroadcastOf  Checks if the address is a broadcast address of a network.
     * @param mask  The network's mask.
     * @return Ipv4Address == (Ipv4Address | ~Mask).
     */
    bool isBroadcastOf(const Ipv4Address &mask, const Ipv4Address &networkDestination);

    /**
     * @brief isMulticast  Checks if the address starts with 0b1110.
     * @return (Ipv4Address & (0xff000000 >> 24)  & 0b11100000) == 0b11100000.
     */
    bool isMulticast() { return (toInt() & 0xe0000000u) == 0xe0000000u; }

    inline Ipv4Address &operator ++();

    /**
     * @brief inRange Check if Ipv4 Address is in an Ipv4 Mask range.
     * @param ipDesination
     * @param ipMask
     * @return bool: The address in the range or not.
     *
     * To check if an Ip address is in Network:
     *  + Calculate the AND bitwise operator between the address
     *    and the ip mask.
     *  + Check if the result is equal to your Network Destination Address.
     *
     * Example:
     *  Network Mask = 255.255.0.0
     *  Destination  = 124.65.3.1
     *  IP Address   = 124.65.32.123
     *
     *  IP & Mask == 124.65.0.0 == Dest.
     */
    bool inRange(const Ipv4Address &ipDestination, const Ipv4Address &ipMask) const;

    static bool isValidIpv4Address(const std::string &ipv4Address);

private:
    /**
     * @brief Convert string ip to intger.
     * @param ipString  string to convertion.
     * @return 32 bit integer of ip address in the host byte order.
     */
    Ipv4AddressIntType ipStringtoInt(const std::string &ipString) const;

    /**
     * @brief ipIntToString Convert ipv4 Integer to string.
     * @param ipInt  integer in the host byte order.
     * @param string  converted string.
     * @throw InvalidArgument and OutOfRange.
     */
    std::string ipIntToString(Ipv4AddressIntType ipInt) const;

    uint32_t mIntegerAddress;
};

} // ProtocolLearn

#endif // IPV4ADDRESS_H
