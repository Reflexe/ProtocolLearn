/**
 * @file PacketStream.h
 * @author shrek0 (shrek0.tk@gmail.com)
  PacketStream
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

#ifndef PACKETSTREAM_H
#define PACKETSTREAM_H

#include <functional>

#include "Timeout.h"
#include "ProtocolFilter.h"

namespace ProtocolLearn {

template<typename _PacketType, typename _FilterType>
class PacketStream
{
public:
    typedef _PacketType PacketType;
    typedef _FilterType FilterType;

    static_assert(std::is_base_of<Packet, PacketType>::value, "PacketType isn't derived from Packet");

    static_assert(std::is_base_of<ProtocolFilter<PacketType>, FilterType>::value,
                  "FilterType isn't derived from the right ProtocolFilter");
    typedef typename FilterType::DropReasonType DropReasonType;
    typedef std::function<void(PacketType &, DropReasonType)> DropHandlerType;

    PacketStream()
    {
    }

    virtual ~PacketStream()
    {
    }

    /**
     * @brief Receive packets until the packet that received meets the requirments.
     * @param packet  Packet to reveive into,  must be in the ProtocolFilter type or derived from it.
     * @throw Timeout::TimeoutException  When the time for receiving passed.
     * @note Use _recv() to receive every packet without filtering.
     * @note If a exception thrown, the packet will initilize.
     */
    virtual void receivePacket(PacketType &packet) {
        auto originalTimeout = mTimeout.getTimeToWait();

        try {
            _receivePacket(packet);
        } catch(...) {
            packet.reset();

            if (getTimeout() != originalTimeout)
                setTimeout(originalTimeout);

            throw;
        }

        if (getTimeout() != originalTimeout)
            setTimeout(originalTimeout);
    }

    void _receivePacket(PacketType &packet) {
        mTimeout.start();

        do {
            _recv(packet);

            auto dropReason = mProtocolFilter.check(packet);
            if(dropReason != FilterType::None) {
                mDropHandler(packet, dropReason);
            } else {
                //Hooray, we have found the right packet!

                pl_debug("Hooray, we have found the right packet!");

                return;
            }

            if (mTimeout.isPassed())
                throw Timeout::TimeoutException{"PacketStream::receivePacket()"};

            if (mTimeout.isInfinite() == false)
                setTimeout(mTimeout.howMuchTimeDoWeHave());
        } while(true);
    }

    /**
     * @brief Send a packet, and update the filter to get the answer.
     * @param packet  Packet for sending, must be in the ProtocolFilter type or derived from it.
     * @note Use _send() to send a packet without filter update.
     */
    virtual void sendPacket(PacketType &packet) {
        _send(packet);

        mProtocolFilter.filterByPacket(packet);
    }

    /**
     * @brief setTimeout  Set the timeout for receivePacket().
     * @param seconds
     * @param useconds
     */
    virtual void setTimeout(const Timeout::TimeType &time)
    {
        mTimeout.setTimeToWait(time);
    }

    const Timeout::TimeType &getTimeout() const{ return mTimeout.getTimeToWait(); }

    FilterType &getFilter() { return mProtocolFilter; }
    const FilterType &getFilter() const{ return mProtocolFilter; }

    /**
     * @brief Just receive a packet (Without any filtering)
     * @param packet  Packet in the ProtocolFilter type or derived from it.
     */
    virtual void _recv(PacketType &packet) = 0;

    /**
     * @brief Just send a packet.
     * @param packet  Packet in the ProtocolFilter type or derived from it.
     */
    virtual void _send(PacketType &packet) = 0;

    static void defaultDropHandler(PacketType &, DropReasonType dropReason)
    {
        pl_debug("Packet dropped, drop reason: " << dropReason);
    }

    void setDropHandler(const DropHandlerType &dropHandler=defaultDropHandler)
    {
        mDropHandler = dropHandler;
    }

    virtual void setMinimumDataLength(OctetVector::SizeType minimumDataLength)
    {
        getFilter().setUnderStreamFilter(true, minimumDataLength);
    }

private:
    FilterType mProtocolFilter;

    /**
     * @brief time  Time for timeout.
     */
    Timeout mTimeout{0, 0};

    /**
     * @brief Called when packet dropped (called by
     *
     * Some protocols (eg. TCP) requires an action on dropping a packet
     * (eg. sending of a RST packet)
     * By default, this function does nothing.
     */
    DropHandlerType mDropHandler = defaultDropHandler;
};

class NoFilter{};

template<typename PacketType>
class PacketStream<PacketType, NoFilter>
{
public:
    PacketStream()
    {
    }

    virtual ~PacketStream()
    {
    }

    virtual void receivePacket(PacketType &packet)
    {
        return _recv(packet);
    }

    virtual void sendPacket(PacketType &packet)
    {
        return _send(packet);
    }

    virtual void _send(PacketType &packet) = 0;
    virtual void _recv(PacketType &packet) = 0;

    virtual void setTimeout(const Timeout::TimeType &time)
    {
        mTimeout.setTimeToWait(time);
    }

    const Timeout::TimeType &getTimeout() const
    {
        return mTimeout.getTimeToWait();
    }

    virtual void setMinimumDataLength(OctetVector::SizeType)
    {
    }

private:
    /**
     * @brief mTimeout  Time for timeout.
     */
    Timeout mTimeout{0, 0};
};

} // ProtocolLearn

#endif // PACKETSTREAM_H
