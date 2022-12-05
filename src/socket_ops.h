//
// Created by 沈昊 on 2022/12/5.
//

#ifndef LIBEV_SOCKET_OPS_H
#define LIBEV_SOCKET_OPS_H

#ifdef __cplusplus
extern "C" {
#endif
int make_fd_nonblock(int fd, int non_blocking);
int make_fd_close_exec(int fd, int on);

#ifdef __cplusplus
};
#endif
#endif//LIBEV_SOCKET_OPS_H
