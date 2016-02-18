project(Ipv4Sniffer)
add_executable(${PROJECT_NAME} main.cpp)

pl_add_module_auto_caller(Layers/Internet/Ipv4/SmartIpv4Stream)

