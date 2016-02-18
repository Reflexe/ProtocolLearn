project(DerivedDataStream)
add_library(${PROJECT_NAME} ${pl_library_type} DerivedDataStream.cpp DerivedDataStream.h)

pl_add_module_auto_caller(Utilities/DataStreamUnderDataStream)
pl_add_module_auto_caller(Utilities/MultiStreamDataStream)
