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

#include "Debug.h"

namespace ProtocolLearn {

constexpr Debug::Debug()
{

}

#ifdef PL_DEBUG

std::unordered_set<std::string> Debug::mDebbugedFiles;

void Debug::assert(bool result, const char *expr, const char *functionName, int line, const char *) {
    if(result)
        return;

    debug << expr <<  " in " << functionName << ":" << line << " Failed." << std::endl;

    throw AssertException(expr);
}

void Debug::addDebbugedFile(const std::string &string) {
    if(mDebbugedFiles.count(string) == 1)
        return;

    mDebbugedFiles.insert(string);
}

void Debug::addDebbugedClass(const std::string &string) {
    std::string path = std::string(PL_DEBUG_BASE_PATH) + "/" + string;
//std::string(PL_DEBUG_BASE_PATH) + "/" + string + "/" + std::string(basename(string.c_str()))
    addDebbugedFile(path);
}

bool Debug::isDebbugedFile(const std::string &string) {
    if(mDebbugedFiles.empty())
        return true;

    return mDebbugedFiles.count(string.substr(0, string.find_last_of('/')));
}

#endif

} // namespace ProtocolLearn

