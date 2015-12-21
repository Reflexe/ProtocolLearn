/**
 * @file %{FILENAME}
 * @author shrek0 (shrek0.tk@gmail.com)
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) %YEAR% shrek0
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

#ifndef PROTOCOLLEARN_BASICINTERFACE_H
#define PROTOCOLLEARN_BASICINTERFACE_H

#include <memory>

#include "MacAddress.h"

#include "Ipv4Address.h"
#include "Ipv6Address.h"

#include "BasicProtocol.h"

namespace ProtocolLearn {

class BasicInterface : public BasicProtocol
{
public:
    BasicInterface();

    virtual MacAddress getMacAddress() = 0;

    // Currently, we don't support multiple addresses for one BasicInterface.
    virtual Ipv4Address getIpv4Address() = 0;
    virtual Ipv4Address getIpv4NetworkMask() = 0;

    virtual Ipv6Address getIpv6Address() = 0;
    virtual Ipv6Address getIpv6NetworkMask() = 0;

    struct BasicInterfaceFork {
        virtual ~BasicInterfaceFork() = default;

        virtual BasicInterface &getBasicInterface() = 0;
    };

    virtual std::unique_ptr<BasicInterfaceFork> fork() = 0;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_BASICINTERFACE_H
