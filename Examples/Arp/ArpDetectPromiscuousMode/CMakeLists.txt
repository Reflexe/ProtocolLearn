project(ArpDetectPromiscuousMode)
add_executable(${PROJECT_NAME} main.cpp)

pl_add_module_auto_caller(Layers/Other/RtNetlink/Route)

pl_add_module_auto_caller(Layers/Link/Ethernet)
pl_add_module_auto_caller(Layers/Other/Arp)
pl_add_module_auto_caller(Layers/Other/Arpv4)

pl_add_module_auto_caller(Utilities/Ipv4Address)
pl_add_module_auto_caller(Utilities/LowSocket)
