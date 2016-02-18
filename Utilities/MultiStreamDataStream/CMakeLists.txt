project(MultiStreamDataStream)
add_library(${PROJECT_NAME} ${pl_library_type} MultiStreamDataStream.cpp MultiStreamDataStream.h PacketCache.cpp PacketCache.h)

pl_add_module_auto_caller(Utilities/DataStreamUnderDataStream)
pl_add_module_auto_caller(Utilities/Timeout)
pl_add_module_auto_caller(Utilities/OctetVector)
