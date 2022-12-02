set(EXTERNAL_INCLUDE_DIRS "")
if(ENABLE_XRADIO_872_SDK)
    set(XRADIO_PREFIX_PATH /home/shenhao/xradiotech-xr872)
    list(APPEND EXTERNAL_INCLUDE_DIRS
            ${CMAKE_SOURCE_DIR}/platform/xr872
            ${XRADIO_PREFIX_PATH}/include/net/lwip-2.0.3/posix
            ${XRADIO_PREFIX_PATH}/include/net/lwip-2.0.3
            ${XRADIO_PREFIX_PATH}/include/net/mbedtls-2.16.0
            ${XRADIO_PREFIX_PATH}/include
            ${XRADIO_PREFIX_PATH}/include/cedarx/os_glue/include
            ${XRADIO_PREFIX_PATH}/include/libc)
    include_directories(${EXTERNAL_INCLUDE_DIRS})
    add_definitions(-DEV_SIGNAL_ENABLE=0 -DEV_CHILD_ENABLE=0 -DECB_NO_SMP)
endif()