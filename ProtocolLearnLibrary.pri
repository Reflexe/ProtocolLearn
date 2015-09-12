include ( ProtocolLearnFunctions.pri )

TEMPLATE = lib
CONFIG *= static
QT =
CONFIG -= QT

# enable c++11
CONFIG *= c++11
CONFIG -= thread
QMAKE_CXXFLAGS += -Wall
TARGET =
