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

#ifndef PROTOCOLLEARN_TCPOPTIONSPARSER_H
#define PROTOCOLLEARN_TCPOPTIONSPARSER_H

#include "TcpIpv4OptionsParser.h"

namespace ProtocolLearn {
namespace Tcp {

struct TcpOptionID : TcpIpv4OptionIDType {
    /**
     * @brief The same as Option Number in IPv4. The numbers in octal for the sake of the RFC793 writer.
     */
    enum OptionKind{
        End = 00, /// No Length
        NOP = 01, /// No Length
        MaximumSegementSize   = 02,/// Has length
        WindowScale           = 3, /// Has length
        SelectiveACKPermitted = 4, /// Has length
        SelectiveACK          = 5, /// Has length
        Timestamp             = 8  /// Has length.
    };

    OptionKind optionKind;

    virtual uint8_t toOctet() const override final;

    virtual void fromOctet(uint8_t octet) override final;
};

class TcpOptionsParser : public TcpIpv4OptionsParser<TcpOptionID>
{
public:
    TcpOptionsParser();

    /**
     * @brief Align the vector to 4 before the returning.
     * @return
     */
    virtual OctetVector toOctetVector() const override final;

    /**
     * @brief Align the vector length to 4.
     * @return
     */
    virtual OctetVector::SizeType getVectorLength() const override final;

private:
    virtual IsLengthOptionReturn isLengthOption(const TcpOptionID &optionID) const override final;
    virtual bool isEndOption(const TcpOptionID &optionID) const override final;
    virtual bool isNoOperationOption(const TcpOptionID &optionID) const override final;
};

} // namespace Tcp
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_TCPOPTIONSPARSER_H
