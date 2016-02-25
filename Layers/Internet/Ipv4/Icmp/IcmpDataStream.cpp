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

#include "IcmpDataStream.h"

#include "Random.h"

namespace ProtocolLearn {
namespace Icmp {

IcmpDataStream::IcmpDataStream(IcmpStream &icmpStream, uint8_t type, uint8_t code)
    : DataStreamUnderPacketStream(icmpStream) {
    mSendPacket.setSequence(Random::getMediumRandomNumber()); // Add some security, 16 bit number is not enough.
    mSendPacket.setId(Random::getMediumRandomNumber());

    mSendPacket.setType(type);
    mSendPacket.setCode(code);

    getPacketStream().getFilter().filterByPacket(mSendPacket);
}

void IcmpDataStream::sendData(OctetVector &&data) {
    mSendPacket.setSequence(mSendPacket.getSequence()+1);

    DataStreamUnderPacketStream::sendData(std::move(data));
}

// What a long name!
OctetVector::SizeType IcmpDataStream::performMaxTransmissionUnitPathDiscovery(const OctetVector::SizeType &minimumAccurecy,
                                                                              const uint16_t maxFails) {
    OctetVector::SizeType maxSentData = 0;
    OctetVector::SizeType minFailedData = getMaximumSendDataLength()+minimumAccurecy;

    OctetVector data;
    uint16_t failsCount = 0;

    while(failsCount < maxFails && (maxSentData+minimumAccurecy) < minFailedData) {
        // If the size that didn't passed before is passed now.
        if(minFailedData < maxSentData) // "They're trying to fool us all!"
            break;

        // Is that called a Binary Search? I not sure; 'P' is such a cool character!
        // Perapes, it's good than '\-0' (One's complement, I HATE you11!) (and what the apes have done to you?)
        data.resize(minFailedData - ((minFailedData - maxSentData)/2), static_cast<uint8_t>('P'));
        pl_assert(data.size() != 0);

        sendData(std::move(data));

        try{
            receiveData();

            // EPIC Programming time! BUUU HAAA!
            if(getReceivePacket().getType() == ICMP_DEST_UNREACH && getReceivePacket().getCode() == ICMP_FRAG_NEEDED)
                // We've got an EROOR! BUHAHAH!
                minFailedData = data.size();
            else
                // We've got back the data, YOOOHOO! HAHHHHH!
                maxSentData = data.size();
        } catch(Timeout::TimeoutException &) {
            ++failsCount; // "Security i- Oops, Hea, *Cyber* Security is one of our most important things"

            continue; // Try again.
        }
    }

    return maxSentData;
}

} // namespace Icmp
} // namespace ProtocolLearn

