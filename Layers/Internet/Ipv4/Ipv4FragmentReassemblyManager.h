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
#ifndef PROTOCOLLEARN_IPV4_IPV4FRAGMENTREASSEMBLYMANAGER_H
#define PROTOCOLLEARN_IPV4_IPV4FRAGMENTREASSEMBLYMANAGER_H

#include "Timeout.h"

#include "Ipv4FragmentReassembler.h"

namespace ProtocolLearn {
namespace Ipv4 {

class Ipv4FragmentReassemblyManager
{
public:
    Ipv4FragmentReassemblyManager();

    /**
     * @brief Enter a packet to the Container.
     * @param ipv4Packet  A rvalue reference to the packet (may change).
     * @return true if the packet is complete, the returned packet will be in the @arg ipv4Packet.
     *
     * This function will try to lookup for an entry for the packet (ID, Source and the same Protocol),
     * If it isn't exist, it'll create one. Then, it'll insert the packet to the reassembler and check if
     * the packet reassembltion process is complete. If so, it'll put the result in @arg ipv4Packet and
     * will return true. If not, it'll just turn false.
     *
     * Another thing that the function does is the checking of the Reassembler Entry's timeout:
     * If it passed, we'll delete it (even if we just received a matching packet).
     *
     * IDEA: Maybe we should check all the reassemblers every time in order to remove timeout-ed
     *       entries? We can't delete it on the time they pass: we've no timers.
     */
    bool enterPacket(Ipv4Packet &&ipv4Packet);

    void tryClearOldEntries();

private:
    static uint64_t hashPacket(const Ipv4Packet &packet);

    struct FragmentReassmblerEntry {
        Timeout timeout;
        Ipv4FragmentReassembler fragmentReassembler;
    };

    std::unordered_map<uint64_t, FragmentReassmblerEntry> mFragmentReassemblersMap;
    Timeout mFragmentionTimeout{0, 33333};
};

} // namespace Ipv4
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_IPV4_IPV4FRAGMENTREASSEMBLYMANAGER_H
