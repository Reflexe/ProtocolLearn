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
#ifndef PROTOCOLLEARN_PEVENT_H
#define PROTOCOLLEARN_PEVENT_H

#include <functional>
#include <list>

namespace ProtocolLearn {

enum class PEventCallbackPlace {
    First,
    Last
};

/// Return value for the callbacks.
//enum class PEventReturn{
//    Continiue,
//    Break
//};

template<class FunctionType>
class PEvent
{
};

template<class ReturnValue, class ...ArgumentsTypes>
class PEvent<ReturnValue(ArgumentsTypes...)>
{
public:
    typedef std::function<ReturnValue(ArgumentsTypes...)> CallbackType;
    typedef typename std::list<CallbackType>::const_iterator CallbackIDType;

    CallbackIDType registerCallback(const CallbackType &callback, PEventCallbackPlace callbackPlace=PEventCallbackPlace::First) {
        switch (callbackPlace) {
        case PEventCallbackPlace::First:
            mCallbacks.push_front(callback);
            return mCallbacks.begin();
            break;
        case PEventCallbackPlace::Last:
            mCallbacks.push_back(callback);
            return --mCallbacks.end();
        default:
            break;
        }
    }

    CallbackIDType registerCallback(CallbackType &&callback, PEventCallbackPlace callbackPlace=PEventCallbackPlace::Last) {
        switch (callbackPlace) {
        case PEventCallbackPlace::First:
            mCallbacks.push_front(std::move(callback));
            return mCallbacks.begin();
            break;
        case PEventCallbackPlace::Last:
            mCallbacks.push_back(std::move(callback));
            return --mCallbacks.end();
        default:
            break;
        }
    }

    void unregisterCallback(const CallbackIDType &callbackID)
    {
        mCallbacks.erase(callbackID);
    }

//    template<class ClassType>
//    void registerCallback(FunctionType *function, ClassType *pClass)
//    {
//        mCallbacks.push_back(std::bind(function, pClass, ));
//    }

    //    typedef ReturnValue FunctionType(ArgumentsTypes...);

    //    template<constexpr std::size_t i, class ...PlaceHoldersType, PlaceHoldersType ...Placeholders>
    //    struct PlaceholdersPack
    //    {
    //        typedef PlaceholdersPack<--i, __OtherScope::Placeholders<i>,_Placeholders...> __Placeholders;
    //    };

    //    template<class ..._Placeholders>
    //    struct PlaceholdersPack<0, _Placeholders...>
    //    {
    //        typedef _Placeholders... type;
    //    };

    //    template<class ...Arguments>
    //    struct GeneratePlaceholders
    //    {
    //        constexpr ...decltype(PlaceholdersPack<sizeof ...Arguments>::Placeholders) type;
    //    };
    //    template<class ...Arguments>
    //    struct GeneratePlaceholders
    //    {
    //        PlaceholdersPack<sizeof ...Arguments> type;
    //    };

    void call(ArgumentsTypes... args) {
        for(const auto &callback : mCallbacks) {
            /*auto returnValue = */callback(args...);

//            if(returnValue == PEventReturn::Break)
//                break;
        }
    }

private:
    std::list<CallbackType> mCallbacks;
};

} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_PEVENT_H
