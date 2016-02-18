project(EthernetSniffer)
add_executable(${PROJECT_NAME} main.cpp)

pl_add_module_auto_caller(Layers/Link/SocketInterface)
pl_add_module_auto_caller(Layers/Link/Ethernet)
pl_add_module_auto_caller(Utilities/Ipv4Address)
