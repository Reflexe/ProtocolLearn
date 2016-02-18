project(Interface)
add_library(${PROJECT_NAME} ${pl_library_type} Interface.cpp Interface.h InterfaceFunctions.cpp  InterfaceFunctions.h)

pl_add_module_auto_caller(Utilities/MacAddress)
pl_add_module_auto_caller(Utilities/Ipv4Address)
pl_add_module_auto_caller(Utilities/Socket) # InterfaceFunctions uses ioctl(2).
pl_add_module_auto_caller(Utilities/ByteOrder)
pl_add_module_auto_caller(Utilities/Ipv6Address)
