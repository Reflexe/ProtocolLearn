include ( ProtocolLearnFunctions.pri )

defineTest(includeProtocolLearnLibrary) {
    libName = $$1

    LIBS *= -L$$PL_LIBRARYS_PATH/$$libName -l$$libName

    export(LIBS)
}

# Include common library.
#includeProtocolLearnLibrary(common)

# Include packet and required utilites.

# Use c++11
#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
CONFIG -= QT
QT =
