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

#ifndef PACKETWRAPPER_H
#define PACKETWRAPPER_H

#include <Packet.h>

namespace ProtocolLearn {

template<class Header, typename PacketType=Packet>
class PacketWrapper : public PacketType
{
public:
    typedef Header HeaderType;

    static const OctetVector::SizeType MinimumHeaderLength = sizeof(Header);

protected:
    PacketWrapper()
        : PacketType(sizeof(Header))
    {
    }

    Header &getHeader() {
        OctetVector &header = PacketType::getVectorHeader();

        return header.getAsObject<Header>();
    }

    const Header &getHeader() const{
        const OctetVector &header = PacketType::getVectorHeader();

        return header.getAsObject<Header>();
    }
};

}

#endif // PACKETWRAPPER_H
