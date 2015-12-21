/**
 * @file rtattrparser.h
 * @author shrek0 (shrek0.tk@gmail.com)
  RtattrParser
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

#ifndef RTATTRPARSER_H
#define RTATTRPARSER_H

#include "OptionsParser.h"

#include <linux/netlink.h>
#include <linux/rtnetlink.h>

namespace ProtocolLearn {

struct myrtattr : OptionType {
    using OptionType::OptionType;

    virtual OctetVector::SizeType getSize() const override final
    {
        return sizeof(rtattr) + OptionType::getSize();
    }
};

class RtattrParser : public OptionsParser<uint16_t, myrtattr>
{
public:
    RtattrParser();

    /**
     * @brief parse  Parse the attributes into attrs map.
     * @param data  Data to parse.
     */
    virtual void parse(const OctetVector &data) override final;

    /**
     * @brief toData  Unparse the data: convert the map to an bytes vector.
     * @return  vector that contain the attributes.
     */
    virtual OctetVector toOctetVector() const override final;
};

}

#endif // RTATTRPARSER_H
