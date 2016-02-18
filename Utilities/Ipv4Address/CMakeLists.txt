project(Ipv4Address)
add_library(${PROJECT_NAME} ${pl_library_type} Ipv4Address.cpp Ipv4Address.h)

pl_add_module_auto_caller(Utilities/StringFunctions)
pl_add_module_auto_caller(Utilities/Convertions)
