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

#ifndef PROTOCOLLEARN_INTERNETPROTOCOL_H
#define PROTOCOLLEARN_INTERNETPROTOCOL_H

#include <memory>

#include "BasicProtocol.h"

namespace ProtocolLearn {

//template<typename PacketStreamType>
//class PacketStreamUnderPacketStream {
//public:
//    PacketStreamUnderPacketStream(PacketStreamType &packetStream)
//        : mPacketStream{packetStream}
//    {
//    }

//    virtual void _recv(typename PacketStreamType::PacketType &packet)
//    {
//        mPacketStream._recv(packet);
//    }

//    virtual void _send(typename PacketStreamType::PacketType &packet)
//    {
//        return mPacketStream._send(packet);
//    }

//    virtual void sendPacket(typename PacketStreamType::PacketType &packet)
//    {
//        return mPacketStream.sendPacket(packet);
//    }

//    virtual void receivePacket(typename PacketStreamType::PacketType &packet)
//    {
//        return mPacketStream.receivePacket(packet);
//    }

//    const PTime::TimeVal &getTimeout() const{ return mPacketStream.getTimeout(); }

//    typename PacketStreamType::FilterType &getFilter() { return mPacketStream.getFilter(); }
//    const typename PacketStreamType::FilterType &getFilter() const{ return mPacketStream.getFilter(); }

//    void setDropHandler(const typename PacketStreamType::DropHandlerType &dropHandler=PacketStreamType::defaultDropHandler)
//    {
//        return mPacketStream.setDropHandler(dropHandler);
//    }

//private:
//    PacketStreamType &mPacketStream;
//};

class InternetProtocol : public BasicProtocol
{
public:
    InternetProtocol();
    virtual ~InternetProtocol() = default;

    // For MTU path discovery.
    virtual OctetVector::SizeType getRealMaximumSendDataLength()
    {
        return getMaximumSendDataLength();
    }

    struct InternetProtocolFork{
        virtual ~InternetProtocolFork() = default;

        virtual InternetProtocol &getInternetProtocol() = 0;
    };

    virtual std::unique_ptr<InternetProtocolFork> fork(bool reply=false) = 0;

//    // Internet should care about fragmention.
//    virtual bool isFragmentionEnabled() const = 0;
//    virtual void setFragmention(bool status) = 0;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_INTERNETPROTOCOL_H
