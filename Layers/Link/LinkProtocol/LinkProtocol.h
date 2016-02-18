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

#ifndef PROTOCOLLEARN_LINKPROTOCOL_H
#define PROTOCOLLEARN_LINKPROTOCOL_H

#include <memory>

#include "BasicProtocol.h"
#include "BasicInterface.h"

namespace ProtocolLearn {

class LinkProtocol : public BasicProtocol
{
public:
    LinkProtocol();

    struct LinkProtocolFork{
        LinkProtocolFork(std::unique_ptr<BasicInterface::BasicInterfaceFork> &&basicInterface);
        virtual ~LinkProtocolFork() = default;

        virtual LinkProtocol &getLinkProtocol() = 0;

    protected:
        std::unique_ptr<BasicInterface::BasicInterfaceFork> mBasicInterfaceFork;
    };

    virtual std::unique_ptr<LinkProtocolFork> fork(bool reply=false) = 0;

    virtual BasicInterface &getBasicInterface() = 0;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_LINKPROTOCOL_H
