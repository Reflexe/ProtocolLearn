cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

set(pl_root ${CMAKE_CURRENT_SOURCE_DIR})
set(pl_library_type STATIC)
set(pl_cpp_ext cpp)

set(pl_debug 1)

if(pl_debug EQUAL "1")
    set(CMAKE_BUILD_TYPE Debug)
else(pl_debug EQUAL "1")
    set(CMAKE_BUILD_TYPE Release)
endif(pl_debug EQUAL "1")

enable_testing()

function(pl_add_module target_path caller_name)
    get_filename_component(target_name ${target_path} NAME)

    if(NOT IS_DIRECTORY "${target_path}/")
       message(FATAL_ERROR "${target_path} is not exist!")
    endif(NOT IS_DIRECTORY "${target_path}/")

    if(NOT TARGET ${target_name})
        message(FATAL_ERROR "${target_name} is not a valid target!")
    endif(NOT TARGET ${target_name})

    target_include_directories(${caller_name} PUBLIC "${pl_root}/${target_path}/")
    target_link_libraries(${caller_name} ${target_name})
endfunction(pl_add_module target_path caller_name)

macro(pl_add_module_auto_caller target_path)
    pl_add_module(${target_path} ${PROJECT_NAME})
endmacro(pl_add_module_auto_caller target_path)

set_property(GLOBAL PROPERTY CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(Utilities)
add_subdirectory(Protocols)
add_subdirectory(Libraries)
add_subdirectory(Examples)
