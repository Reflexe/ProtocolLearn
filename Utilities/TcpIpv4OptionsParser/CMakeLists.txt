project(TcpIpv4OptionsParser)
add_library(${PROJECT_NAME} ${pl_library_type} TcpIpv4OptionsParser.cpp TcpIpv4OptionsParser.h)

pl_add_module_auto_caller(Utilities/OptionsParser)
