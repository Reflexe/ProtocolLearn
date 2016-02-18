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

#ifndef PROTOCOLLEARN_IPPROTOCOLIPV4_H
#define PROTOCOLLEARN_IPPROTOCOLIPV4_H

#include "Ipv4DataStream.h"

#include "IPProtocolStream.h"

namespace ProtocolLearn {
namespace Ipv4 {

class IPProtocolIpv4 : public IPProtocolStream
{
public:
    IPProtocolIpv4(Ipv4DataStream &ipv4DataStream);

private:
    struct Ipv4PseudoHeader{
        uint32_t sourceAddress;
        uint32_t destinationAddress;
        uint8_t reserved;
        uint8_t protocol;
        uint16_t segementLength;
    }__attribute__((packed));

    virtual void updatePseudoHeaderRecv(IPProtocolPacket &packet) override;
    virtual void updatePseudoHeaderSend(IPProtocolPacket &packet) override;

    virtual std::unique_ptr<IPProtocolStreamFork> fork(bool reply=false) override;

    void updatePseudoHeaderByPacket(OctetVector &pseudoHeader, const Ipv4Packet &ipv4Packet, OctetVector::SizeType segementLength);

    Ipv4DataStream &mIpv4DataStream;
};

} // namespace Ipv4
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_IPPROTOCOLIPV4_H
