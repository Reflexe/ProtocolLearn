cmake_minimum_required(VERSION 2.8)

set(pl_root ${CMAKE_CURRENT_SOURCE_DIR})
set(pl_library_type STATIC)
set(pl_cpp_ext cpp)

enable_testing()

# No no o PL is PERL?
function(pl_add_module target_path caller_name)
    get_filename_component(target_name ${target_path} NAME)

    if(NOT EXISTS "${target_path}/")
       message(FATAL_ERROR "${target_path} is not exist!")
    endif(NOT EXISTS "${target_path}/")

    include_directories("${pl_root}/${target_path}/")
    target_link_libraries(${caller_name} ${target_name})
endfunction(pl_add_module target_path caller_name)

include_directories("${pl_root}/Include")
include_directories(".")
set_property(GLOBAL PROPERTY CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(Utilities)
add_subdirectory(Protocols)
add_subdirectory(Libraries)
add_subdirectory(Examples)
