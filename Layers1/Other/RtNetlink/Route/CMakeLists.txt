project(Route)
add_library(${PROJECT_NAME} ${pl_library_type} Route.cpp Route.h)

pl_add_module_auto_caller(Layers/Other/Netlink)
pl_add_module_auto_caller(Layers/Other/RtNetlink)

pl_add_module_auto_caller(Utilities/Ipv4Address)
pl_add_module_auto_caller(Utilities/Interface)
pl_add_module_auto_caller(Utilities/MessageSocket)
pl_add_module_auto_caller(Utilities/ByteOrder)
