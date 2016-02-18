project(Ipv6Address)
add_library(${PROJECT_NAME} ${pl_library_type} Ipv6Address.cpp Ipv6Address.h)

pl_add_module_auto_caller(Utilities/StringFunctions)
pl_add_module_auto_caller(Utilities/Convertions)
