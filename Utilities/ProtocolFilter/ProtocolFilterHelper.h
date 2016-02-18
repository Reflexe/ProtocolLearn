/**
 * @file ProtocolFilterHelper.h
 * @author shrek0 (shrek0.tk@gmail.com)
  ProtocolFilterHelper
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
 * Contain a class that provide commoon Protocol Filtering.
 */

#ifndef PROTOCOLFILTERHELPER_H
#define PROTOCOLFILTERHELPER_H

#include "Packet.h"

namespace ProtocolLearn {

class ProtocolFilterHelper
{
public:


    ProtocolFilterHelper() {
        // By default, all the filters turns off except the protocolFilter and the previous packet filter.

        filtersStatus.protocolFilter = filtersStatus.previousPacketFilter = true;
        filtersStatus.underStreamFilter = filtersStatus.userFilter = false;
    }


    /* Filter's status Disable-Enable Functions */

    /**
     * @brief Set the status and the minimum data size for the packet.
     * @param status  The new status for the filter (true: on, false: off).
     * @param minimumDataLength  Minimum data size for the packet.
     */
    void setUnderStreamFilter(bool status, OctetVector::SizeType minimumDataLength=0) {
        filtersStatus.underStreamFilter = status;
        underStreamMinimumDataLength = minimumDataLength;
    }

    /**
     * @brief Set the Protocol Filter's status.
     * @param status  The new status for the filter (true: on, false: off).
     */
    void setProtocolFilterStatus(bool status)
    {
        filtersStatus.protocolFilter = status;
    }

    /**
     * @brief setPreviousPacketFilterStatus  Set the Previous Packet Filter's status.
     * @param status
     */
    void setPreviousPacketFilterStatus(bool status)
    {
        filtersStatus.previousPacketFilter = status;
    }

    /**
     * @brief Set the User Defined Filter's status.
     * @param status  The new status for the filter (true: on, false: off).
     */
    void setUserFilterStatus(bool status)
    {
        filtersStatus.userFilter = status;
    }

    /**
     * @brief
     * @return The Under Stream Filter's status (true: on, false: off).
     */
    bool getUnderStreamFilterState()
    {
        return filtersStatus.underStreamFilter;
    }

    /**
     * @brief
     * @return The Protocol Filter's status (true: on, false: off).
     */
    bool getProtocolFilterState()
    {
        return filtersStatus.protocolFilter;
    }

    /**
     * @brief
     * @return The Previous Packet Filter's status (true: on, false: off).
     */
    bool getPreviousPacketFilterStatus()
    {
        return filtersStatus.previousPacketFilter;
    }

    /**
     * @brief getUserFilterStatus
     * @return The User Defined Filter's status (true: on, false: off).
     */
    bool getUserFilterStatus()
    {
        return filtersStatus.userFilter;
    }

    void filterByPacket(const Packet &packet);

private:


protected:
    template <class PacketType>
    static const PacketType &protocolfilterCast(const Packet &packet) {
#ifdef PL_DEBUG
        pl_assert(dynamic_cast<const PacketType*>(&packet) != nullptr);

        return static_cast<const PacketType &>(packet);
#else
        return static_cast<const PacketType&>(packet);
#endif

    }

    /**
     * @brief checkByPreviousPacket  Check if the packet is a response for the previous packet.
     * @param filteredPacket  The packet for the test.
     * @param filteringPacket  The previous packet.
     * @return yes (true) or not (false)
     */
    virtual DropReasonType checkByPreviousPacket(const Packet &filteredPacket) = 0;

    /**
     * @brief checkByProtocol  Check if the packet meets the protocol's condetions (e.g. a valid checksum).
     * @param filteredPacket  The packet for the test.
     * @return yes (true) or not (false)
     */
    virtual DropReasonType checkByProtocol(const Packet &filteredPacket) = 0;

    /**
     * @brief checkByUserRules  Check if the packet meets the user's condetions.
     * @param filteredPacket  The packet for the test.
     * @return yes (true) or not (false)
     */
    virtual DropReasonType checkByUserRules(const Packet &filteredPacket) = 0;

    /**
     * @brief filterByProtocolPacket  Save the required details from the previous packet in order to
     *                                receive the right packet with the previous packet filter.
     * @param filteringPacket  The previous packet.
     */
    virtual void filterByPacket(const Packet &filteringPacket) = 0;
};

} // ProtocolLearn

#endif // PROTOCOLFILTERHELPER_H
