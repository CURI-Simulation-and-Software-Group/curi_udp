cmake_minimum_required(VERSION 3.2.0)
project(curi_udp)

###################################Add library###########################################
add_library(CURI_UDP_LIB
    ${PROJECT_SOURCE_DIR}/curi_udp.c
)
target_include_directories(CURI_UDP_LIB PUBLIC 
    ${PROJECT_SOURCE_DIR}
)

###################################Generate Executable###################################
#Generate Executable if this CMakeLists isn't a subdirectory 
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    link_libraries(CURI_UDP_LIB)
    add_executable(local
        ${CMAKE_SOURCE_DIR}/local.c
    )
    add_executable(remote
        ${CMAKE_SOURCE_DIR}/remote.c
    )
endif()