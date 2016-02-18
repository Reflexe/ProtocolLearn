#ifndef STRINGFUNCTIONS_H
#define STRINGFUNCTIONS_H

#include <string>
#include <list>

namespace ProtocolLearn {

class StringFunctions
{
public:
    StringFunctions();

    /**
     * @brief StringFunctions::split  Split string to parts by another string.
     * @param string  String to split.
     * @param delim  String to split by.
     * @return list of the splited strings.
     */
    static std::list<std::string> split(const std::string &string, const std::string &delim);
};

} // ProtocolLearn

#endif // STRINGFUNCTIONS_H
