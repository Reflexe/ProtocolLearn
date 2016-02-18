project(Debug)
add_library(${PROJECT_NAME} ${pl_library_type} Debug.cpp Debug.h)

pl_add_module_auto_caller(Utilities/Exceptions)

configure_file(DebugConfig.h.gen DebugConfig.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
