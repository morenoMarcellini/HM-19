cmake_minimum_required(VERSION 3.1.0)
set(PROJECT_NAME Bluetooth)
project(${PROJECT_NAME})

add_definitions(-std=c++14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
enable_language(CXX)

# xml stuff
include(FindPkgConfig)
pkg_search_module(XML2 REQUIRED libxml-2.0)

set(COMMON_SOURCE
	${CMAKE_SOURCE_DIR}/src/Bluetooth.C
	${CMAKE_SOURCE_DIR}/src/SerialPort.C
	${CMAKE_SOURCE_DIR}/src/xmlConfiguration.C
	${CMAKE_SOURCE_DIR}/main.C
)

set(PROJECT_CONFIG_DIR ${CMAKE_SOURCE_DIR}/include)

#file(GLOB_RECURSE Bluetooth_SRC "*.C")
#file(GLOB_RECURSE Bluetooth_HEADERS "*.H")
add_compile_options(-g -O2 -pipe -march=native)
# executable
add_executable(${PROJECT_NAME} ${COMMON_SOURCE}) # ${Bluetooth_SRC} ${Bluetooth_HEADERS})
# post-build
add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/include ${XML2_INCLUDE_DIRS})
link_directories(${XML2_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} ${XML2_LIBRARIES})#add_library(CppLinuxSerial ${CppLinuxSerial_SRC} ${CppLinuxSerial_HEADERS})
#target_include_directories(CppLinuxSerial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# On Linux, "sudo make install" will typically copy the library
# into the folder /usr/local/bin
#install(TARGETS  Bluetooth DESTINATION lib)

# On Linux, "sudo make install" will typically copy the
# folder into /usr/local/include
#install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/CppLinuxSerial DESTINATION include)
