//
// Created by 沈昊 on 2022/12/5.
//
#include "socket_ops.h"
#if ENABLE_XRADIO_872_SDK
#include <lwip/sockets.h>
#include <sys/_default_fcntl.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#endif
int make_fd_nonblock(int fd, int non_blocking) {
#if defined(_WIN32)
    unsigned long ul = noblock;
#else
    int ul = non_blocking;
#endif                                //defined(_WIN32)
    int ret = ioctl(fd, FIONBIO, &ul);//设置为非阻塞模式
    if (ret == -1) {
        return ret;
    }
    return 0;
}

int make_fd_close_exec(int fd, int on) {
#undef fcntl
#if !defined(_WIN32)
    int flags = fcntl(fd, F_GETFD);
    if (flags == -1) {
        return -1;
    }
    if (on) {
        flags |= FD_CLOEXEC;
    } else {
        int cloexec = FD_CLOEXEC;
        flags &= ~cloexec;
    }
    int ret = fcntl(fd, F_SETFD, flags);
    if (ret == -1) {
        return -1;
    }
    return ret;
#else
    return -1;
#endif
}