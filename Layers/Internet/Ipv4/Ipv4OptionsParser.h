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

#ifndef PROTOCOLLEARN_PROTOCOLS_IPV4_IPV4OPTIONSPARSER_H
#define PROTOCOLLEARN_PROTOCOLS_IPV4_IPV4OPTIONSPARSER_H

#include "TcpIpv4OptionsParser.h"

#include "BitField.h"

namespace ProtocolLearn {
namespace Ipv4 {

/**
 * @brief An IPv4 option looks like this:
 *         ---------------------------------------------------------------------------------------------------------------
 * Octets |                                                       0                                                       |
 * Bits   | 0           | 1           | 2           | 3           | 4           | 5           | 6           | 7           |
 *         ---------------------------------------------------------------------------------------------------------------
 *        | Copied Flag |       Option Class        |                           Option Number                             |
 *         ---------------------------------------------------------------------------------------------------------------
 *
 * Copied Flag (boolean): On fragmention, copy it to all the fragments?
 */
struct Ipv4OptionIDType : TcpIpv4OptionIDType {
    enum OptionClass {
        Control = 0U,
        Debug = 2U
    };

    enum OptionNumber {
        // Control:
        End         = 0U,   /// The end of the option list, not length (but one octet after).
        NoOperation = 1U,   /// ?
        Security    = 2U,   /// 11 bytes.
        LooseRoute  = 3U,
        StrictRoute = 9U,
        RecordRoute = 7U,
        StreamID    = 8U,   /// 4 Bytes.

        // Debug:
        Timestamp   = 4U
    };

    typedef BitField<uint8_t> BitFieldType;

    virtual void fromOctet(uint8_t octet) override final{
        copiedFlag = BitFieldType::getPosition<0, 1>(octet);
        optionClass = static_cast<OptionClass>(BitFieldType::getPosition<1, 2>(octet));
        optionNumber = static_cast<OptionNumber>(BitFieldType::getPosition<3, 5>(octet));
    }

    virtual uint8_t toOctet() const override final{
        uint8_t returnValue = 0;

        BitFieldType::setPosition<0, 1>(returnValue, copiedFlag);
        BitFieldType::setPosition<1, 2>(returnValue, optionClass);
        BitFieldType::setPosition<3, 5>(returnValue, optionClass);

        return returnValue;
    }

    bool copiedFlag;
    OptionClass optionClass;
    OptionNumber optionNumber;
};

/**
 * @brief The Ipv4OptionsParser class  Impomention of the Ipv4 options parsing.
 *
 * @see RFC791, page 15.
 */
class Ipv4OptionsParser : public TcpIpv4OptionsParser<Ipv4OptionIDType>
{
public:
    typedef Ipv4OptionIDType::OptionNumber OptionNumber;
    typedef Ipv4OptionIDType::OptionClass OptionClass;

    Ipv4OptionsParser();

    /**
     * @brief setOption  Set the copied flag to false before the setting.
     * @param optionID
     * @param optionValue
     */
    virtual void setOption(uint8_t optionID, const OptionStructType &optionValue) override final;

    virtual bool isOptionExist(uint8_t optionID) const override final;

private:
    virtual IsLengthOptionReturn isLengthOption(const Ipv4OptionIDType &optionID) const override final;

    virtual bool isEndOption(const Ipv4OptionIDType &optionID) const override final;
    virtual bool isNoOperationOption(const Ipv4OptionIDType &optionID) const override final;

};

} // namespace Ipv4
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_PROTOCOLS_IPV4_IPV4OPTIONSPARSER_H
