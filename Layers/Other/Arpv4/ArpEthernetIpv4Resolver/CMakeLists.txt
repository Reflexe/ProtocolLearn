project(ArpEthernetIpv4Resolver)
add_library(${PROJECT_NAME} ${pl_library_type} ArpEthernetIpv4Resolver.cpp ArpEthernetIpv4Resolver.h)

pl_add_module_auto_caller(Layers/Link/SocketInterface)

pl_add_module_auto_caller(Layers/Other/RtNetlink/Route)

pl_add_module_auto_caller(Layers/Other/Arp)
pl_add_module_auto_caller(Layers/Other/Arpv4)
pl_add_module_auto_caller(Layers/Link/Ethernet)
