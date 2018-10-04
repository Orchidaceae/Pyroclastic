cmake_minimum_required(VERSION 3.10)
project(Pyroclastic)

set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
include_directories(include/glfw)

set(SOURCE_FILES main.cpp debug_vulkan.h)
add_executable(Pyroclastic ${SOURCE_FILES})

if(WIN32)
    # Get rid of annoying MSVC warnings.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    
    # Turn on using solution folders for VS
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Dumps the .DLLs at the same place as the .exe depending on the build type 
    add_custom_command(TARGET Pyroclastic POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/bin
        ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
endif(WIN32)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})
target_link_libraries(Pyroclastic ${Vulkan_LIBRARIES})

if(WIN32)
    find_library(GLFW_LIBRARIES NAMES glfw glfw3.lib PATHS ${CMAKE_SOURCE_DIR}/libs)
    target_link_libraries(Pyroclastic ${GLFW_LIBRARIES})
else(WIN32)
    find_package(GLFW REQUIRED)
    include_directories(${GLFW_INCLUDE_DIRS})
    target_link_libraries(Pyroclastic ${GLFW_LIBRARIES})
endif(WIN32)