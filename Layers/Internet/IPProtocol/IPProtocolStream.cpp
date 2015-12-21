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

#include "IPProtocolStream.h"

namespace ProtocolLearn {

IPProtocolStream::IPProtocolStream(InternetProtocol &internetProtocol)
    : PacketStreamUnderDataStream{internetProtocol}
{
}

void IPProtocolStream::_recv(IPProtocolPacket &packet) {
    PacketStreamUnderDataStream::_recv(packet);

    updatePseudoHeaderRecv(packet);
}

void IPProtocolStream::_send(IPProtocolPacket &packet) {
    updatePseudoHeaderSend(packet);

    PacketStreamUnderDataStream::_send(packet);
}

IPProtocolStream::IPProtocolStreamFork::IPProtocolStreamFork(std::unique_ptr<InternetProtocol::InternetProtocolFork> &&internetProtocolFork)
    : mInternetProtocol{std::move(internetProtocolFork)}
{
}

} // namespace ProtocolLearn

