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

#ifndef PROTOCOLLEARN_OPTIONSPARSER_H
#define PROTOCOLLEARN_OPTIONSPARSER_H

#include <unordered_map>

#include "CompilerFunctions.h"
#include "OctetVector.h"

namespace ProtocolLearn {

/**
 * @brief Base sturct for options that uses OptionsParser.
 */
struct OptionType {
    OptionType(const OctetVector &data);

    OptionType(OctetVector &&data);

    OptionType();
    PL_DECLARE_DEFAULT_VIRTUAL_DISRUCTOR(OptionType)

    OctetVector data;

    virtual OctetVector::SizeType getSize() const;
};

/**
 * @brief A base class for parsering and settings of attributes.
 */
template<typename _OptionIDType, typename _OptionStructType=OptionType>
class OptionsParser
{
public:
    static_assert(std::is_integral<_OptionIDType>::value, "_OptionIDType isn't an integral type");
    static_assert(std::is_base_of<OptionType, _OptionStructType>::value
                  || std::is_same<_OptionStructType, OptionType>::value,
                  "OptionType isn't base of _OptionStructType");

    typedef _OptionIDType OptionIDType;
    typedef _OptionStructType OptionStructType;

    typedef std::unordered_map<OptionIDType, OptionStructType> MapType;

    OptionsParser()
    {
    }

    PL_DECLARE_DEFAULT_VIRTUAL_DISRUCTOR(OptionsParser)

    /**
     * @brief parse  Parse the given range into mOptionsMap.
     */
    virtual void parse(OctetVector::const_iterator begin, OctetVector::const_iterator end) = 0;

    /**
     * @brief toOctetVector  Export mOptionsMap into a OctetVector.
     * @return
     */
    virtual OctetVector toOctetVector() const = 0;

    virtual bool isOptionExist(OptionIDType optionID) const
    {
        return mOptionsMap.count(optionID);
    }

    virtual void setOption(OptionIDType optionID, const OptionStructType &optionValue)
    {
        mOptionsMap[optionID] = optionValue;
    }

    template<typename T>
    void setOption(OptionIDType optionID, T value) {
        OptionStructType option;
        option.data = OctetVector::fromObject(value);

        setOption(optionID, option);
    }

    virtual const OptionStructType &getOption(OptionIDType optionID) const
    {
        return mOptionsMap.at(optionID);
    }

    virtual bool isValidOptions() const
    {
        return true;
    }

    template<typename T>
    T getOption(OptionIDType optionID) const{
        pl_assert(sizeof(T) <= mOptionsMap.at(optionID).data.size());

        const OctetVector &temp = mOptionsMap.at(optionID).data;

        return temp.toObject<T>();
    }

    void removeOption(const OptionIDType &optionID)
    {
        mOptionsMap.erase(optionID);
    }

    void clearOptions()
    {
        mOptionsMap.clear();
    }

    /**
     * @brief getVectorLength  Counts the octets in the options.
     * @return
     * @note The return of this function is not reliable (Integer Overflow).
     */
    virtual OctetVector::size_type getVectorLength() const{
        OctetVector::size_type length = 0;

        for(const auto &keyAndOption : mOptionsMap)
        {
            length += keyAndOption.second.getSize();
        }

        return length;
    }

    const MapType &getMap() const{ return mOptionsMap; }

protected:
    MapType mOptionsMap;

};


} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_OPTIONSPARSER_H
