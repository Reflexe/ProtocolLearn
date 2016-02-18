project(Ipv4DataStreamExample)
add_executable(${PROJECT_NAME} main.cpp)


pl_add_module(Libraries/Route ${PROJECT_NAME})
pl_add_module(Libraries/ArpEthernetIpv4Resolver ${PROJECT_NAME})
pl_add_module(Protocols/Ethernet ${PROJECT_NAME})
pl_add_module(Protocols/Ipv4 ${PROJECT_NAME})
