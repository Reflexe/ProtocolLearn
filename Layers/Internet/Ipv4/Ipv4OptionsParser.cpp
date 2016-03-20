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

#include "Ipv4OptionsParser.h"

#include "Debug.h"

#include <limits>

namespace ProtocolLearn {
namespace Ipv4 {

Ipv4OptionsParser::Ipv4OptionsParser()
{
}

void Ipv4OptionsParser::setOption(uint8_t optionID, const OptionsParser::OptionStructType &optionValue) {
    Ipv4OptionIDType structOptionID;
    structOptionID.fromOctet(optionID);

    structOptionID.copiedFlag = false; // We have this field in the saved option too, this is just a key.

    return TcpIpv4OptionsParser::setOption(structOptionID.toOctet(), optionValue);
}

bool Ipv4OptionsParser::isOptionExist(uint8_t optionID) const{
    Ipv4OptionIDType structOptionID;
    structOptionID.fromOctet(optionID);

    structOptionID.copiedFlag = false;

    return TcpIpv4OptionsParser::isOptionExist(structOptionID.toOctet());
}

Ipv4OptionsParser::IsLengthOptionReturn Ipv4OptionsParser::isLengthOption(const Ipv4OptionIDType &optionID) const{
    switch(optionID.optionClass) {
    case OptionClass::Control:
        switch (optionID.optionNumber) {
        case OptionNumber::End:
        case OptionNumber::NoOperation:
            return IsLengthOptionReturn::False;
        case OptionNumber::Security:
        case OptionNumber::LooseRoute:
        case OptionNumber::StrictRoute:
        case OptionNumber::RecordRoute:
        case OptionNumber::StreamID:
        default:
            return IsLengthOptionReturn::NotFound;
        }

    case OptionClass::Debug:
        switch (optionID.optionNumber) {
        case OptionNumber::Timestamp:
            return IsLengthOptionReturn::True;
        default:
            return IsLengthOptionReturn::NotFound;
        }

    default:
        return IsLengthOptionReturn::NotFound;
    }
}

bool Ipv4OptionsParser::isEndOption(const Ipv4OptionIDType &optionID) const
{
    return optionID.optionClass == OptionClass::Control && optionID.optionNumber == OptionNumber::End;
}

bool Ipv4OptionsParser::isNoOperationOption(const Ipv4OptionIDType &optionID) const
{
    return optionID.optionClass == OptionClass::Control && optionID.optionNumber == OptionNumber::NoOperation;
}

} // namespace Ipv4
} // namespace ProtocolLearn
