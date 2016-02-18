#include "StringFunctions.h"

namespace ProtocolLearn {

StringFunctions::StringFunctions()
{
}

std::list<std::string> StringFunctions::split(const std::string &string, const std::string &delim) {
    std::list<std::string> result;
    std::string temp = "";
    std::string::size_type subStart = 0, subEnd = 0;

    // If the delim is empty, return the original string.
    if (delim.empty()) {
        result.push_back(string);
        return result;
    }

    do{
        // Find the next delim.
        subEnd = string.find(delim, subStart);

        // If we didn't found next delim.
        if(subEnd == std::string::npos)
            // Create a substring from subStart to @arg string's end.
            temp = string.substr(subStart);
        else
            // Create a substring from subStart to subend.
            temp = string.substr(subStart, subEnd-subStart);

        if (!temp.empty())
            result.push_back(temp);

        // subStart will be the subEnd + delim length. (in order to avoid no-end loop)
        subStart = subEnd + delim.length();
    } while(subEnd != std::string::npos);

    return result;
}

} // ProtocolLearn
