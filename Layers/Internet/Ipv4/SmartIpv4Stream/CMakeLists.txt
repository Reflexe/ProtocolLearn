project(SmartIpv4Stream)
add_library(${PROJECT_NAME} ${pl_library_type} SmartIpv4Stream.cpp SmartIpv4Stream.h)

pl_add_module_auto_caller(Layers/Other/Arpv4/ArpEthernetIpv4Resolver)
pl_add_module_auto_caller(Layers/Other/RtNetlink/Route)

pl_add_module_auto_caller(Layers/Link/SocketInterface)
pl_add_module_auto_caller(Layers/Link/Ethernet)
pl_add_module_auto_caller(Layers/Internet/Ipv4)

