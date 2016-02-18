project(Ping)
add_executable(${PROJECT_NAME} main.cpp)

pl_add_module_auto_caller(Layers/Internet/Ipv4)
pl_add_module_auto_caller(Layers/Internet/Ipv4/SmartIpv4Stream)
pl_add_module_auto_caller(Utilities/PTime)

