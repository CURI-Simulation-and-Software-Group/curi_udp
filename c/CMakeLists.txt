cmake_minimum_required(VERSION 3.2.0)
project(curi_udp)

if (CMAKE_VERSION VERSION_LESS "3.2.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

if (CMAKE_CL_64)
    set(ADDRESS_MODEL 64)
    set(NODE_TARGET x64)
else()
    set(ADDRESS_MODEL 32)
    set(NODE_TARGET x86)
endif()

if (WIN32)
    message("OS: Windows")
    add_executable(local
        ${CMAKE_SOURCE_DIR}/curi_udp.h
        ${CMAKE_SOURCE_DIR}/curi_udp.c
        ${CMAKE_SOURCE_DIR}/local.c
    )
    add_executable(remote
        ${CMAKE_SOURCE_DIR}/curi_udp.h
        ${CMAKE_SOURCE_DIR}/curi_udp.c
        ${CMAKE_SOURCE_DIR}/remote.c
    )
elseif (UNIX AND NOT APPLE)
    message("OS: Linux")
    add_executable(local
        ${CMAKE_SOURCE_DIR}/curi_udp.h
        ${CMAKE_SOURCE_DIR}/curi_udp.c
        ${CMAKE_SOURCE_DIR}/local.c
    )
    add_executable(remote
        ${CMAKE_SOURCE_DIR}/curi_udp.h
        ${CMAKE_SOURCE_DIR}/curi_udp.c
        ${CMAKE_SOURCE_DIR}/remote.c
    )
endif()
