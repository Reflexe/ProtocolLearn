project(Packet)
add_library(${PROJECT_NAME} ${pl_library_type} Packet.cpp Packet.h)

pl_add_module_auto_caller(Utilities/Exceptions)
pl_add_module_auto_caller(Utilities/Debug)
pl_add_module_auto_caller(Utilities/OctetVector)
pl_add_module_auto_caller(Utilities/CompilerFunctions)
