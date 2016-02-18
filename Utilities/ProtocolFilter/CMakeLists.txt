project(ProtocolFilter)
add_library(${PROJECT_NAME} ${pl_library_type} ProtocolFilter.cpp ProtocolFilter.h)

pl_add_module_auto_caller(Utilities/Packet)
