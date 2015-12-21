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

#ifndef PROTOCOLLEARN_IPPROTOCOLSTREAM_H
#define PROTOCOLLEARN_IPPROTOCOLSTREAM_H

#include <memory>

#include "PacketStreamUnderDataStream.h"
#include "InternetProtocol.h"

#include "IPProtocolPacket.h"

namespace ProtocolLearn {

/// Update the pseudo header before the R/S.
class IPProtocolStream : public PacketStreamUnderDataStream<IPProtocolPacket, NoFilter, InternetProtocol>
{
public:
    IPProtocolStream(InternetProtocol &internetProtocol);

    virtual void _recv(IPProtocolPacket &packet) override;
    virtual void _send(IPProtocolPacket &packet) override;

    virtual void updatePseudoHeaderSend(IPProtocolPacket &packet) = 0;
    virtual void updatePseudoHeaderRecv(IPProtocolPacket &packet) = 0;

    struct IPProtocolStreamFork{
        IPProtocolStreamFork(std::unique_ptr<InternetProtocol::InternetProtocolFork> &&internetProtocolFork);

        virtual IPProtocolStream &getIPProtocol() = 0;

        virtual ~IPProtocolStreamFork() = default;
        std::unique_ptr<InternetProtocol::InternetProtocolFork> mInternetProtocol;
    };

    virtual std::unique_ptr<IPProtocolStreamFork> fork(bool reply=false) = 0;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_IPPROTOCOLSTREAM_H
