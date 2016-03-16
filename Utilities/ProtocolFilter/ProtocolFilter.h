/**
 * @file ProtocolFilter.h
 * @author shrek0 (shrek0.tk@gmail.com)
  ProtocolFilter
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

#ifndef PROTOCOLFILTER_H
#define PROTOCOLFILTER_H

#include <functional>
#include <type_traits> // std::is_base_of

#include "Packet.h"

namespace ProtocolLearn {

template<class PacketType>
class ProtocolFilter
{
public:
    enum DropReason {
        InvalidLength = 0xfffd, /* Under stream filter failed */
        None          = 0xfffe, /* There's no drop */
        NoReason      = 0xffff  /* There's no reason for this drop */
    };

    typedef uint16_t DropReasonType;

    static_assert(std::is_base_of<Packet, PacketType>::value, "PacketType isn't Derived from Packet");

    typedef std::function<DropReasonType(const PacketType &)> UserDefinedFilterFunctionType;

    ProtocolFilter() {
        mFiltersStatus.protocolFilter = mFiltersStatus.previousPacketFilter = true;
        mFiltersStatus.underStreamFilter = mFiltersStatus.userFilter = false;
    }

    /**
     * @brief Check with the filters if the packet should be passed.
     * @param filteredPacket  The packet to check.
     * @return the DropReason.
     */
    DropReasonType check(const PacketType &filteredPacket) {
        DropReasonType dropReason = DropReason::None; // By Default - without any filter, every packet passes.

        if(dropReason == None && mFiltersStatus.underStreamFilter) {
            dropReason = checkByUnderStreamFilter(filteredPacket);
            pl_debug("Checking by Under Stream Filter... " << (dropReason == None ? "Passed" : "Failed"));
        }

        if(dropReason == None && mFiltersStatus.protocolFilter) {
            dropReason = checkByProtocol(filteredPacket);
            pl_debug("Checking by Protocol Filter... " << (dropReason == None ? "Passed" : "Failed"));
        }

        if(dropReason == None && mFiltersStatus.previousPacketFilter) {
            dropReason = checkByPreviousPacket(filteredPacket);
            pl_debug("Checking by Previous Packet Filter... " << (dropReason == None ? "Passed" : "Failed"));
        }

        if(dropReason == None && mFiltersStatus.userFilter) {
            dropReason = checkByUserFilter(filteredPacket);
            pl_debug("Checking by User Filter... " << (dropReason == None ? "Passed" : "Failed"));
        }

        return dropReason;
    }

    /**
     * @brief Set the status and the minimum data size for the packet.
     * @param status  The new status for the filter (true: on, false: off).
     * @param minimumDataLength  Minimum data size for the packet.
     */
    void setUnderStreamFilter(bool status, OctetVector::SizeType minimumDataLength=0) {
        mFiltersStatus.underStreamFilter = status;
        underStreamMinimumDataLength = minimumDataLength;
    }

    OctetVector::SizeType getMinimumDataLength() const
    {
        return underStreamMinimumDataLength;
    }

    /**
     * @brief Set the Protocol Filter's status.
     * @param status  The new status for the filter (true: on, false: off).
     */
    void setProtocolFilterStatus(bool status)
    {
        mFiltersStatus.protocolFilter = status;
    }

    /**
     * @brief setPreviousPacketFilterStatus  Set the Previous Packet Filter's status.
     * @param status
     */
    void setPreviousPacketFilterStatus(bool status)
    {
        mFiltersStatus.previousPacketFilter = status;
    }

    void setUserFilterStatus(bool status, const UserDefinedFilterFunctionType &function=UserDefinedFilterFunctionType()) {
        mUserDefinedFilter = function;

        mFiltersStatus.userFilter = status;
    }

    const UserDefinedFilterFunctionType &getUserDefinedFilterFunction() const
    {
        return mUserDefinedFilter;
    }

    /**
     * @brief
     * @return The Under Stream Filter's status (true: on, false: off).
     */
    bool getUnderStreamFilterStatus() const
    {
        return mFiltersStatus.underStreamFilter;
    }

    /**
     * @brief
     * @return The Protocol Filter's status (true: on, false: off).
     */
    bool getProtocolFilterStatus() const
    {
        return mFiltersStatus.protocolFilter;
    }

    /**
     * @brief
     * @return The Previous Packet Filter's status (true: on, false: off).
     */
    bool getPreviousPacketFilterStatus() const
    {
        return mFiltersStatus.previousPacketFilter;
    }

    /**
     * @brief getUserFilterStatus
     * @return The User Defined Filter's status (true: on, false: off).
     */
    bool getUserFilterStatus() const
    {
        return mFiltersStatus.userFilter;
    }

    virtual void filterByPacket(const PacketType &)
    {
    }

protected:
    DropReasonType checkByUnderStreamFilter(const PacketType &packet)
    {
        return packet.getDataLength() >= underStreamMinimumDataLength ? None : InvalidLength;
    }

    DropReasonType checkByUserFilter(const PacketType &packet)
    {
        return mUserDefinedFilter(packet);
    }

    virtual DropReasonType checkByProtocol(const PacketType &)
    {
        return DropReason::None;
    }

    virtual DropReasonType checkByPreviousPacket(const PacketType &)
    {
        return DropReason::None;
    }

private:
    UserDefinedFilterFunctionType mUserDefinedFilter;

    struct FiltersStatus {
        bool underStreamFilter;
        bool protocolFilter;
        bool previousPacketFilter;
        bool userFilter;
    };

    /**
     * @brief The under stream minimum data length, for the under stream filter.
     *
     */
    OctetVector::SizeType underStreamMinimumDataLength;

    /**
     * @brief Contains the filters's statuses.
     */
    FiltersStatus mFiltersStatus;
};

}

#endif // PROTOCOLFILTERWRAPPER_H
