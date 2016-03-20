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

#ifndef PROTOCOLLEARN_TCPIPV4OPTIONSPARSER_H
#define PROTOCOLLEARN_TCPIPV4OPTIONSPARSER_H

#include <limits>

#include "CompilerFunctions.h"
#include "OptionsParser.h"

namespace ProtocolLearn {

struct TcpIpv4OptionIDType {
    TcpIpv4OptionIDType() = default;

    PL_DECLARE_DEFAULT_VIRTUAL_DISRUCTOR(TcpIpv4OptionIDType)

    /**
     * @brief Export the option ID to an octet.
     * @return
     */
    virtual uint8_t toOctet() const = 0;

    /**
     * @brief Import the optionID from an octet.
     * @param octet
     */
    virtual void fromOctet(uint8_t octet) = 0;
};

/**
 * @brief Base class for other options.
 */
template<typename OptionIDType>
struct TcpIpv4OptionType : OptionType {
    OptionIDType optionID;

    virtual OctetVector::SizeType getSize() const override
    {   // The data's size, the option id's size and (if there's data) the length byte.
        return OptionType::getSize() + sizeof(optionID) + (data.size() == 0 ? 0 : 1);
    }
};

template<typename StructOptionIDType, typename OptionIDType=uint8_t, typename OptionType=TcpIpv4OptionType<StructOptionIDType>>
class TcpIpv4OptionsParser : public OptionsParser<OptionIDType, OptionType>
{
public:
    static_assert(std::is_base_of<TcpIpv4OptionIDType, StructOptionIDType>::value,
                  "StructOptionIDType must be derived of TcpIpv4OptionIDType");

    static_assert(std::is_same<OptionType, TcpIpv4OptionType<StructOptionIDType>>::value
                  || std::is_base_of<OptionType, TcpIpv4OptionType<StructOptionIDType>>::value,
                  "OptionType must be derived of TcpIpv4OptionType<StructOptionIDType>");

    typedef OptionsParser<OptionIDType, OptionType> OptionsParserType;

    enum class InvalidationReason {
        None,
        NotEnoughSpaceForOptionID,
        NotEnoughSpaceForOptionLength,
        NotEnoughSpaceForOptionData,
        InvalidOptionID,
        DuplicatedOptionID
    };

    TcpIpv4OptionsParser()
    {
    }

    PL_DECLARE_DEFAULT_VIRTUAL_DISRUCTOR(TcpIpv4OptionsParser)

    virtual void parse(OctetVector::const_iterator begin, OctetVector::const_iterator end) override{
        OptionsParserType::clearOptions();
        setInvalidOptions(InvalidationReason::None);

        pl_debug("Parsering options...");

        if(end-begin < static_cast<OctetVector::difference_type>(sizeof(OptionIDType)))
            return setInvalidOptions(InvalidationReason::NotEnoughSpaceForOptionID);

        for(auto nextOctet = begin; nextOctet < end; ) {
            StructOptionIDType optionID{};
            optionID.fromOctet(*nextOctet);
            ++nextOctet;

            OptionType newOption;
            newOption.optionID = optionID;

            switch (isLengthOption(optionID)) {
            case IsLengthOptionReturn::True:
            {
                // If there's no place for the option length byte.
                if(end == nextOctet) {
                    pl_debug("no place for option length");
                    return setInvalidOptions(InvalidationReason::NotEnoughSpaceForOptionLength);
                }

                const uint8_t optionLength = *nextOctet;
                ++nextOctet;

                if(isValidOptionLength(optionLength, end, nextOctet) == false) {
                    pl_debug("Invalid option length: " << +optionLength);
                    return setInvalidOptions(InvalidationReason::NotEnoughSpaceForOptionData);
                }

                if(optionLength != 2) {
                    // Insert the option data. We're stating from nextOctet to nextOctet+optionLength-(sizeof(OptionIDType)+sizeof(OptionLengthType).
                    newOption.data.assign(nextOctet, nextOctet+(optionLength-2));
                    nextOctet += (optionLength-2);
                }

                break;
            }
            case IsLengthOptionReturn::False:
                if(isEndOption(optionID) == true)
                    return;
                else if(isNoOperationOption(optionID) == true)
                    continue;
                break;
            case IsLengthOptionReturn::NotFound:
                // If it's an invalid option number;
                pl_debug("option not found: " << +*nextOctet);
                return setInvalidOptions(InvalidationReason::InvalidOptionID);
                break;
            }

            // Prevent from overrideing options.
            if(OptionsParserType::isOptionExist(optionID.toOctet()) == true) {
                pl_debug("The option already exist!");
                return setInvalidOptions(InvalidationReason::DuplicatedOptionID);
            }

            OptionsParserType::setOption(optionID.toOctet(), newOption);
        }
    }

    virtual OctetVector toOctetVector() const override{
        OctetVector octetVector;

        for(const auto &option : OptionsParserType::getMap()) {
            const auto &optionID = option.second.optionID;

            octetVector.push_back(optionID.toOctet());

            if(isLengthOption(optionID) == IsLengthOptionReturn::True) {
                pl_assert(option.second.data.size() > 0);
                pl_assert((option.second.data.size()+2) <= std::numeric_limits<uint8_t>::max());

                octetVector.push_back(static_cast<uint8_t>(option.second.data.size()+2));

                octetVector.add(option.second.data);
            } else {
                pl_assert(option.second.data.size() == 0);
            }
        }

        octetVector.alignTo(4);

        return octetVector;
    }

    OctetVector::size_type getVectorLength() const override{
        auto vectorLength = OptionsParserType::getVectorLength();

        // Align the length.
        if(vectorLength % 4 != 0)
            vectorLength += 4 - (vectorLength % 4);

        return vectorLength;
    }

    virtual bool isValidOptions() const override
    {
        return mInvalidationReason == InvalidationReason::None;
    }

protected:
    enum class IsLengthOptionReturn {
        True,
        False,
        NotFound
    };

    virtual IsLengthOptionReturn isLengthOption(const StructOptionIDType &optionID) const = 0;
    virtual bool isEndOption(const StructOptionIDType &optionID) const = 0;

    virtual bool isNoOperationOption(const StructOptionIDType &) const
    {
        return false;
    }

private:
    static bool isValidOptionLength(uint8_t optionLength, const OctetVector::const_iterator end, const OctetVector::const_iterator &nextOctet) {
        // The option size field counts the option type and the option length and the data length as well (RFC791, page 15).
        // Therefore, it must be grether than the size of option type and option length.

        // The option size plus the current position must be lower than the the vector's end.
        // The -2 is because that the option length counts itself and the option type octet as well (and we already read them with nextOctet).

        return (optionLength >= 2) && end >= nextOctet+(optionLength-2);
    }

    void setInvalidOptions(InvalidationReason invalidationReason)
    {
        mInvalidationReason = invalidationReason;
    }

    InvalidationReason mInvalidationReason = InvalidationReason::None;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_TCPIPV4OPTIONSPARSER_H
