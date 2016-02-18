project(GetArpEthernetIpv4)
add_executable(${PROJECT_NAME} main.cpp)

pl_add_module_auto_caller(Layers/Other/Arpv4/ArpEthernetIpv4Resolver)
