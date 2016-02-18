/**
 * @file Packet.h
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
 * Contain a class that provide easy way to packet managing.
 */

#ifndef PACKET_H
#define PACKET_H

#include "OctetVector.h"

#include "Debug.h"

namespace ProtocolLearn {

/**
 * @brief A type for easly creation of packets.
 */
class Packet
{
public:
    /**
     * @brief Regular constractor.
     * @param headerLength  headers length.
     */
    explicit Packet(const OctetVector::SizeType &minimumHeaderLength);

    virtual ~Packet() = default;

//    Packet(const Packet &packet) = default;
//    Packet(Packet &&packet) = default;

//    Packet &operator = (Packet &&packet) = default;
//    Packet &operator = (const Packet &packet) = default;

    /**
     * @brief Reset the packet to inital state.
     */
    void reset();

    /**
     * @brief Remove the data part, the packet will be header only.
     */
    void removeData();

    /**
     * @brief Import packet from a vector.
     * @param rawPacket  Vector of bytes.
     */
    void fromRawPacket(const OctetVector &rawPacket, const OctetVector::SizeType &headerLength);

    void fromRawPacket(const OctetVector &rawPacket)
    {
        return fromRawPacket(rawPacket, getMinimumHeaderLength());
    }

    void fromPacket(Packet &packet);

    /**
     * @brief Get the packet in a OctetVector (without calling onPacketExport()).
     * @return
     */
    OctetVector toOctetVector() const;

    /**
     * @brief Get the packet in a OctetVector and call onPacketExport().
     * @return
     */
    OctetVector toVectorRawPacket();

    void onExport();

    /**
     * @brief Get the data.
     * @return OctetVector filled with the packet's data.
     */
    const OctetVector &getVectorData() const
    {
        return d.data;
    }

    OctetVector &getVectorData()
    {
        return d.data;
    }

    const OctetVector &getVectorHeader() const
    {
        return d.header;
    }

    OctetVector &getVectorHeader()
    {
        return d.header;
    }

    /**
     * @brief Check if the packet has data.
     * @return getPacketLength() != 0.
     */
    bool isDataPacket() const
    {
        return getDataLength() != 0;
    }

    /**
     * @brief Import data from a OctetVector.
     * @param data  Data type.
     * @param callOnDataChanged
     */
    void importData(const OctetVector &data);

    void importData(OctetVector &&data);

    /**
     * @brief Import data from a packet.
     * @param packet
     */
    void importData(Packet &packet);

    OctetVector::SizeType getPacketLength() const
    {
        return getDataLength() + getHeaderLength();
    }

    OctetVector::SizeType getDataLength() const
    {
        return d.data.size();
    }

    OctetVector::SizeType getHeaderLength() const
    {
        return d.header.size();
    }

    OctetVector::SizeType getMinimumHeaderLength() const
    {
        return minimumHeaderLength;
    }

    static bool isValidSizeType(const OctetVector::SizeType &dataSizeType);

protected:
    /**
     * @brief Import the header from a data type.
     * @param dataType
     */
    void importHeader(const OctetVector &dataType);

    /**
     * @brief Import a data type to the unrequired part of the header (> minimumHeaderLength).
     * @param dataType
     */
    void importUnrequiredHeader(const OctetVector &dataType);

    OctetVector getUnrequiredHeader();

    /**
     * @brief Called on fromRawPacket.
     */
    virtual void onPacketImport();

    /**
     * @brief Called before a Packet Exporting.
     */
    virtual void onPacketExport();

private:
    void init();

    void resizeOctetVector(OctetVector &dataType, const OctetVector::SizeType &newLength) {
        pl_assert(isValidSizeType(newLength));

        dataType.resize(newLength, 0);
    }

    void resizeData(const OctetVector::SizeType &newLength)
    {
        resizeOctetVector(d.data, newLength);
    }
    void resizeHeader(const OctetVector::SizeType &newLength)
    {
        resizeOctetVector(d.header, newLength);
    }

    struct Data {
        OctetVector header;
        OctetVector data;
    };

    Data d;

    OctetVector::SizeType minimumHeaderLength;
};

} // ProtocolLearn

#endif // PACKET_H
