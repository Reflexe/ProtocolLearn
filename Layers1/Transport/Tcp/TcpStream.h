/**
 * @file
 * @author shrek0 (shrek0.tk@gmail.com)
  TcpStream
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

#ifndef TCPSTREAM_H
#define TCPSTREAM_H

#include "PacketStreamUnderPacketStream.h"

#include "TcpPacket.h"
#include "TcpFilter.h"

#include "IPProtocolStream.h"

namespace ProtocolLearn {
namespace Tcp {

/**
 * @brief Abstact implomantion of Tcp for IPv4 and IPv6.
 *
 * IPv4 and IPv6 has different Pseudo Headers. The functions _recv() and _send()
 * should be overriden to update this pseudo header before it given to the filter.
 */
class TcpStream : public PacketStreamUnderPacketStream<IPProtocolStream, PacketStream<TcpPacket, TcpFilter>>
{
public:
    explicit TcpStream(IPProtocolStream &dataStream);

    void setTcpState(TcpFilter::TcpState state) { getFilter().setTcpState(state); }
    TcpFilter::TcpSession &getTcpSession() { return getFilter().getSession(); }
};

} // Tcp
} // ProtocolLearn

#endif // TCPSTREAM_H
