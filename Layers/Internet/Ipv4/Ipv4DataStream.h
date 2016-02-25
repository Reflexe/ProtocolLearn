/**
 * @file Ipv4DataStream.h
 * @author shrek0 (shrek0.tk@gmail.com)
  Ipv4DataStream
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

#ifndef IPV4DATASTREAM_H
#define IPV4DATASTREAM_H

#include <unordered_map>
#include <memory>

#include "DataStreamUnderPacketStream.h"

#include "Ipv4Stream.h"

#include "InternetProtocol.h"

#include "Ipv4FragmentReassemblyManager.h"

namespace ProtocolLearn {
namespace Ipv4 {

class Ipv4DataStream : public DataStreamUnderPacketStream<Ipv4Stream, InternetProtocol>
{
public:
    Ipv4DataStream(Ipv4Stream &ipv4Stream, const Ipv4Address &destination,
                   uint8_t protocol, const Ipv4Address &source, bool enableFragmention=false);

    class FragmentionNeeded : public RuntimeError
    {
        using RuntimeError::RuntimeError;
    };

    virtual void _send(OctetVector &&data) override final;
    virtual OctetVector _recv() override final; 

    virtual OctetVector::SizeType getMaximumSendDataLength() override final;
    virtual OctetVector::SizeType getRealMaximumSendDataLength() override final;

    bool isFragmentionEnabled() const;
    void setFragmentionStatus(bool status);

    void trySaveMemory();

    virtual std::unique_ptr<InternetProtocolFork> fork(bool reply=false) override;
private:
    bool receiveWithFragmention(Ipv4Packet &packet);
    void sendWithFragmention(Ipv4Packet &packet);

    bool mIsFragmentionEnabled = false;
    std::unique_ptr<Ipv4FragmentReassemblyManager> mpFragmentReassemblers;

};

} // Ipv4
} // ProtocolLearn

#endif // IPV4DATASTREAM_H
