cmake_minimum_required (VERSION 3.16)
project(libev C)

option(ENABLE_EXAMPLE "enable example" ON)
option(ENABLE_XRADIO_872_SDK "mbed on xradio-872 platform" ON)
if(ENABLE_XRADIO_872_SDK)
	add_definitions(-DENABLE_XRADIO_872_SDK -DLWIP_PROVIDE_ERRNO)
	add_compile_options(-Wunused-value)
endif()

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(cmake/platform.cmake)
set(package_name "libev")
set(package_bugreport "")
set(package_bugreport "")
set(libtool_objdir ".libs")

set(libname "libev")
set(version_major "4")
set(version_minor "15")


# check headers
check_include_files(inttypeas.h HAVE_INTTYPES_H)
check_include_files(dlfcn.h HAVE_DLFCN_H)
check_include_files(memory.h HAVE_MEMORY_H)
check_include_files(poll.h HAVE_POLL_H)
check_include_files(port.h HAVE_PORT_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(strings.h HAVE_STRINGS_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(sys/epoll.h HAVE_SYS_EPOLL_H)
check_include_files(sys/eventfd.h HAVE_SYS_EVENTFD_H)
check_include_files(sys/event.h HAVE_SYS_EVENT_H)
check_include_files(sys/inotify.h HAVE_SYS_INOTIFY_H)
check_include_files(sys/select.h HAVE_SYS_SELECT_H)
check_include_files(sys/signalfd.h HAVE_SYS_SIGNALFD_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files("assert.h;ctype.h;errno.h;float.h;limits.h;locale.h;math.h;setjmp.h;signal.h;stdarg.h;stddef.h;stdio.h;stdlib.h;string.h;time.h;" STDC_HEADERS)
check_include_files("winsock.h" HAVE_WINSOCK_H)

# check functions
check_function_exists(clock_gettime HAVE_CLOCK_GETTIME)
check_function_exists(epoll_ctl HAVE_EPOLL_CTL)
check_function_exists(eventfd HAVE_EVENTFD)
check_function_exists(floor HAVE_FLOOR)
check_function_exists(inotify_init HAVE_INOTIFY_INIT)
check_function_exists(kqueue HAVE_KQUEUE)
check_function_exists(nanosleep HAVE_NANOSLEEP)
check_function_exists(poll HAVE_POLL)
check_function_exists(port_create HAVE_PORT_CREATE)
check_function_exists(select HAVE_SELECT)
check_function_exists(signalfd HAVE_SIGNALFD)
if(HAVE_PIPE)
	add_definitions(-DHAVE_PIPE)
endif()
# check system library
check_library_exists(rt pthread_create "" HAVE_LIBRT)

configure_file(src/config.h.cmake.in ${CMAKE_SOURCE_DIR}/src/config.h)

list(APPEND include_dirs "${CMAKE_CURRENT_SOURCE_DIR}")
list(APPEND include_dirs "${CMAKE_CURRENT_BINARY_DIR}")

# libev doesn't do dll export so shared wont work without modifying the code (for msvc)
set(libtype STATIC)

set(src
	"src/ev.c"
	"src/event.c")

if(ENABLE_XRADIO_872_SDK)
	list(APPEND src platform/xr872/pipe.c)
endif()

if(WIN32)
	list(APPEND platformlibs "wsock32.lib" "ws2_32.lib")
endif()


add_library(${libname} ${libtype} ${src} ${internal_hdrs})
target_link_libraries(${libname} ${platformlibs} ${libs})
target_include_directories(${libname} PUBLIC ${CMAKE_SOURCE_DIR}/src
								      PRIVATE ${EXTERNAL_INCLUDE_DIRS})

if(MSVC)
	set_target_properties(${libname} PROPERTIES
		COMPILE_FLAGS "/wd4244 /wd4996")
endif()


if(ENABLE_EXAMPLE)
	add_subdirectory(example)
endif()