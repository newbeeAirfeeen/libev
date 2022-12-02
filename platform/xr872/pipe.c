//
// Created by 沈昊 on 2022/12/2.
//
#include "pipe.h"
#include "errno.h"
#include <lwip/sockets.h>
int pipe(int pipefd[2]) {
    struct sockaddr_in addr = {0};
    int addr_size = sizeof(addr);
    struct sockaddr_in adr2;
    int adr2_size = sizeof(adr2);
    int listener;
    int sock[2] = {-1, -1};

    if ((listener = lwip_socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return -1;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;

    if (bind(listener, (struct sockaddr *) &addr, addr_size))
        goto fail;

    if (getsockname(listener, (struct sockaddr *) &addr, &addr_size))
        goto fail;

    if (listen(listener, 1))
        goto fail;

    if ((sock[0] = lwip_socket(AF_INET, SOCK_STREAM, 0)) == -1)
        goto fail;

    if (connect(sock[0], (struct sockaddr *) &addr, addr_size))
        goto fail;

    if ((sock[1] = accept(listener, 0, 0)) < 0)
        goto fail;

    /* windows vista returns fantasy port numbers for sockets:
   * example for two interconnected tcp sockets:
   *
   * (Socket::unpack_sockaddr_in getsockname $sock0)[0] == 53364
   * (Socket::unpack_sockaddr_in getpeername $sock0)[0] == 53363
   * (Socket::unpack_sockaddr_in getsockname $sock1)[0] == 53363
   * (Socket::unpack_sockaddr_in getpeername $sock1)[0] == 53365
   *
   * wow! tridirectional sockets!
   *
   * this way of checking ports seems to work:
   */
    if (getpeername(sock[0], (struct sockaddr *) &addr, &addr_size))
        goto fail;

    if (getsockname(sock[1], (struct sockaddr *) &adr2, &adr2_size))
        goto fail;

    errno = ENFILE;
    if (addr_size != adr2_size || addr.sin_addr.s_addr != adr2.sin_addr.s_addr /* just to be sure, I mean, it's windows */
        || addr.sin_port != adr2.sin_port)
        goto fail;

    closesocket(listener);

#if EV_SELECT_IS_WINSOCKET
    filedes[0] = EV_WIN32_HANDLE_TO_FD(sock[0]);
    filedes[1] = EV_WIN32_HANDLE_TO_FD(sock[1]);
#else
    /* when select isn't winsocket, we also expect socket, connect, accept etc.
   * to work on fds */
    pipefd[0] = sock[0];
    pipefd[1] = sock[1];
#endif

    return 0;

fail:
    closesocket(listener);

    if (sock[0] != -1) closesocket(sock[0]);
    if (sock[1] != -1) closesocket(sock[1]);

    return -1;
}
