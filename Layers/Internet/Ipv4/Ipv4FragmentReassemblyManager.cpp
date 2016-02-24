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
#include "Ipv4FragmentReassemblyManager.h"

namespace ProtocolLearn {
namespace Ipv4 {

Ipv4FragmentReassemblyManager::Ipv4FragmentReassemblyManager()
{

}

bool Ipv4FragmentReassemblyManager::enterPacket(Ipv4Packet &&ipv4Packet) {
    auto hashPacketResult = hashPacket(ipv4Packet);
    auto findResult = mFragmentReassemblersMap.find(hashPacketResult);

    if (findResult == mFragmentReassemblersMap.end()) {
        findResult = mFragmentReassemblersMap.emplace(hashPacketResult,
                                                      FragmentReassmblerEntry{mFragmentionTimeout, Ipv4FragmentReassembler{}}).first;
    }

    auto &reassemblerEntry = findResult->second;

    if (reassemblerEntry.timeout.isPassed()) {
        mFragmentReassemblersMap.erase(findResult);

        return false;
    }

    if (reassemblerEntry.fragmentReassembler.isPacketRequired(ipv4Packet))
        reassemblerEntry.fragmentReassembler.insertPacket(std::move(ipv4Packet));

    if (reassemblerEntry.fragmentReassembler.isAllOffsetsFilled()) {
        ipv4Packet = reassemblerEntry.fragmentReassembler.toPacket();

        return true;
    } else {
        return false;
    }
}

void Ipv4FragmentReassemblyManager::tryClearOldEntries() {
    for(auto iterator = mFragmentReassemblersMap.begin();
        iterator != mFragmentReassemblersMap.end();
        ++iterator) {
        if(iterator->second.timeout.isPassed() == true)
            mFragmentReassemblersMap.erase(iterator);
    }
}

uint64_t Ipv4FragmentReassemblyManager::hashPacket(const Ipv4Packet &packet) {
    return (packet.getSource().toInt()
            | (static_cast<uint64_t>(packet.getIdentification()) << 32)
            | (static_cast<uint64_t>(packet.getProtocol()) << 48));
    // And then we have 8 bit left.
}

} // namespace Ipv4
} // namespace ProtocolLearn
