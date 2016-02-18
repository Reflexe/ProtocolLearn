project(DataStreamUnderDataStream)
add_library(${PROJECT_NAME} ${pl_library_type} DataStreamUnderDataStream.cpp DataStreamUnderDataStream.h)

pl_add_module_auto_caller(Utilities/DataStream)
