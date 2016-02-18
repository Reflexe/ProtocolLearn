pl_add_library(PacketWrapper PacketWrapper)

pl_add_module_auto_caller(Utilities/Packet)

add_executable("${PROJECT_NAME}TestE" PacketTest.cpp MyPacket.cpp MyPacket.h)
pl_add_module("${PROJECT_NAME}TestE" Utilities/ByteOrder Utilities/PacketWrapper Utilities/Exceptions Utilities/Debug)
add_test("${PROJECT_NAME}Test" "${PROJECT_NAME}TestE")
