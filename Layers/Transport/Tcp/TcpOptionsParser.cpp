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

#include "TcpOptionsParser.h"

namespace ProtocolLearn {
namespace Tcp {

TcpOptionsParser::TcpOptionsParser()
{
}

OctetVector TcpOptionsParser::toOctetVector() const{
    auto octetVector = TcpIpv4OptionsParser::toOctetVector();

    octetVector.alignTo(4);

    return octetVector;
}

OctetVector::SizeType TcpOptionsParser::getVectorLength() const{
    auto vectorLength = TcpIpv4OptionsParser::getVectorLength();

    // Round the vector length to 4.
    if(vectorLength % 4 != 0)
        vectorLength += 4 - (vectorLength % 4);

    return vectorLength;
}

TcpOptionsParser::IsLengthOptionReturn TcpOptionsParser::isLengthOption(const TcpOptionID &optionID) const{
    switch (optionID.optionKind) {
    case TcpOptionID::End:
    case TcpOptionID::NOP:
        return IsLengthOptionReturn::False;
        break;

    case TcpOptionID::MaximumSegementSize:
    case TcpOptionID::WindowScale:
    case TcpOptionID::SelectiveACKPermitted:
    case TcpOptionID::SelectiveACK:
    case TcpOptionID::Timestamp:
        return IsLengthOptionReturn::True;
        break;
    default:
        return IsLengthOptionReturn::NotFound;
        break;
    }
}

bool TcpOptionsParser::isEndOption(const TcpOptionID &optionID) const
{
    return optionID.optionKind == TcpOptionID::End;
}

bool TcpOptionsParser::isNoOperationOption(const TcpOptionID &optionID) const
{
    return optionID.optionKind == TcpOptionID::NOP;
}

uint8_t TcpOptionID::toOctet() const
{
    return static_cast<uint8_t>(optionKind);
}

void TcpOptionID::fromOctet(uint8_t octet)
{
    optionKind = static_cast<OptionKind>(octet);
}


} // namespace Tcp
} // namespace ProtocolLearn

