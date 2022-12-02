cmake_minimum_required (VERSION 2.8.12)

project(libev C)



set(XRADIO_PREFIX_PATH /home/shenhao/xradiotech-xr872)
include_directories(${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${XRADIO_PREFIX_PATH}/include/net/lwip-2.0.3/posix
        ${XRADIO_PREFIX_PATH}/include/net/lwip-2.0.3/
        ${XRADIO_PREFIX_PATH}/include/net/mbedtls-2.16.0/
        ${XRADIO_PREFIX_PATH}/include/
        ${XRADIO_PREFIX_PATH}/include/cedarx/os_glue/include
        ${XRADIO_PREFIX_PATH}/include/libc)

option(ENABLE_EXAMPLE "enable example" ON)

add_subdirectory(src)


if(ENABLE_EXAMPLE)
    add_subdirectory(example)
endif()