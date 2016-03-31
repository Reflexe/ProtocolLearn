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
#ifndef PROTOCOLLEARN_SMARTVECTOR_H
#define PROTOCOLLEARN_SMARTVECTOR_H

#include <cstdint>

#include <vector>
#include <deque>

namespace ProtocolLearn {

template<class _Type=uint8_t,
         class _VectorType=std::vector<_Type>,
         class _ContainerType=std::deque<_VectorType>>
class SmartVectorIterator
{
    typedef typename _VectorType::iterator VectorIterator;
    typedef typename _ContainerType::iterator ContainerIterator;
    typedef typename VectorIterator::diffrence_type DiffrenceType;

    SmartVectorIterator(const VectorIterator &vectorIterator,
                        const ContainerIterator &containerIterator)
        : mCurrentVector{containerIterator}, mCurrent{vectorIterator}
    {
    }

    SmartVectorIterator &operator ++() {
        mCurrent = add_positive(1);
        return *this;
    }

    SmartVectorIterator &operator --() {
        mCurrent = add_netegive(-1);
        return *this;
    }

    SmartVectorIterator &operator +=(DiffrenceType diffrence) {
        mCurrent = add_generic(diffrence);
        return *this;
    }

    SmartVectorIterator &operator -=(DiffrenceType diffrence) {
        // We're adding it, but it should be decrased.
        mCurrent = add_generic(-diffrence);
        return *this;
    }

    SmartVectorIterator operator +(DiffrenceType diffrence) {
        auto thisCopy = *this;

        thisCopy += diffrence;
        return thisCopy;
    }

    SmartVectorIterator operator -(DiffrenceType diffrence) {
        auto thisCopy = *this;

        thisCopy -= diffrence;
        return thisCopy;
    }

    bool operator ==(const SmartVectorIterator &smartVectorIterator) {
        return smartVectorIterator.mCurrentVector == mCurrentVector
                && smartVectorIterator.mCurrent == mCurrent;
    }

    bool operator !=(const SmartVectorIterator &smartVectorIterator)
    {
        return !(smartVectorIterator == *this);
    }

private:
    VectorIterator add_generic(DiffrenceType diffrence)
    {
        return diffrence < 0 ? add_netegive(diffrence) : add_positive(diffrence);
    }

    VectorIterator add_positive(DiffrenceType diffrence) {
        pl_assert(diffrence >= 0);

        auto currentVectorSize = mCurrentVector->end()-mCurrent;

        if(currentVectorSize >= diffrence) {
            return mCurrent+diffrence;
        } else {
            diffrence -= currentVectorSize;
            ++mCurrentVector;
        }

        while ((currentVectorSize = mCurrentVector->size()) < diffrence) {
            diffrence -= currentVectorSize;
            pl_assert(diffrence >= 0);

            ++mCurrentVector;
        }

        return mCurrentVector->begin() + diffrence;
    }

    VectorIterator add_netegive(DiffrenceType diffrence) {
        pl_assert(diffrence < 0);
        diffrence = -diffrence;

        auto currentVectorSize = mCurrent-mCurrentVector->begin();
        if (currentVectorSize >= diffrence) {
            return mCurrent-diffrence;
        } else {
            diffrence -= currentVectorSize;
            --mCurrentVector;
        }

        while ((currentVectorSize = mCurrentVector->size()) < diffrence) {
            diffrence -= currentVectorSize;
            pl_assert(diffrence < 0);

            --mCurrentVector;
        }

        return mCurrentVector->end()-diffrence;
    }

    ContainerIterator mCurrentVector;
    VectorIterator mCurrent;
};

template<class _Type=uint8_t,
         class _VectorType=std::vector<_Type>,
         class _ContainerType=std::deque<_VectorType>>
class SmartVector
{
public:
    typedef _Type Type;
    typedef _VectorType VectorType;
    typedef _ContainerType ContainerType;

    SmartVector()
    {}

private:
    _ContainerType mVectorsContainer;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_SMARTVECTOR_H
