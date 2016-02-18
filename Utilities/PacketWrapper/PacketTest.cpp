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

#include <string>
#include <iostream>

#include "MyPacket.h"
#include "Exceptions.h"

namespace ProtocolLearn {

bool isPacketsEqual(MyPacket &myPacket, MyPacket &anotherMyPacket) {
    pl_assert(anotherMyPacket.getAnotherField() == myPacket.getAnotherField());
    pl_assert(anotherMyPacket.getBigField() == myPacket.getBigField());
    pl_assert(anotherMyPacket.getField() == myPacket.getField());

    pl_assert(anotherMyPacket.getMinimumHeaderLength() == myPacket.getMinimumHeaderLength());
    pl_assert(anotherMyPacket.getDataLength() == myPacket.getDataLength());
    pl_assert(anotherMyPacket.getHeaderLength() == myPacket.getHeaderLength());
    pl_assert(anotherMyPacket.getVectorData() == myPacket.getVectorData());
    pl_assert(anotherMyPacket.toVectorRawPacket() == myPacket.toVectorRawPacket());
    pl_assert(anotherMyPacket.getVectorHeader() == myPacket.getVectorHeader());

    return true;
}

}

int main(void) {
    using namespace ProtocolLearn;

    using ::std::cout;
    using ::std::endl;

    OctetVector data;

    {
        ::std::string str = "StringStringHelloWorldAnotherWordwordWordAAAAAAAAAAAAAAAAAAAAAAA"
                            "AAAAWordWordWordWordWordWordWordWordWordWordWordWordWordWordWord"
                            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
                            "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
        data.insert(data.begin(), str.begin(), str.end());
    }

    MyPacket myPacket;

    // Check fromRawPacket.
    cout << "Checking data fromRawPacket..." << endl;
    myPacket.fromRawPacket(data);

    pl_assert(myPacket.toOctetVector() == data);

    cout << "Checking data importing..." << endl;
    myPacket.importData(data);

    // Check data importing.
    pl_assert(myPacket.getVectorData() == data);

    /* Check Copy */
    cout << "Checking copy constructor..." << endl;

    MyPacket anotherMyPacket = myPacket;

    pl_assert(isPacketsEqual(myPacket, anotherMyPacket));

    myPacket.reset();

    {
        bool exceptionThrown = false;

        try
        {   // Importing of minimumDataSize-1 will throw a excption.
            myPacket.fromRawPacket(data, myPacket.getMinimumHeaderLength()-1);
        }
        catch(InvalidArgument &)
        {
            exceptionThrown = true;
        }
        cout << "Checking fromRawPacket(minimumHeaderLength-1)" << endl;
        pl_assert(exceptionThrown == true);
    }

    {
        cout << "Checking copy operator..." << endl;
        myPacket.reset();
        myPacket = anotherMyPacket;

        pl_assert(isPacketsEqual(myPacket, anotherMyPacket));
    }

    {
        cout << "Checking move operator.." << endl;

        //Backup anotherMyData
        MyPacket anotherMyPacketBackup = anotherMyPacket;

        // Move operator.
        myPacket = std::move(anotherMyPacket);

        // Another my Packet is empty, we moved it.
        pl_assert(isPacketsEqual(myPacket, anotherMyPacketBackup));
    }

    cout << "Done." << endl;

    return EXIT_SUCCESS;
}
