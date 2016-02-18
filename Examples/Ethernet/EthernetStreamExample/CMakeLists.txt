project(EthernetStreamExample)
add_executable(${PROJECT_NAME} main.cpp)

pl_add_module_auto_caller(Layers/Link/Ethernet)

