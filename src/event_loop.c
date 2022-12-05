//
// Created by 沈昊 on 2022/12/5.
//
#include "event_loop.h"
#include <pthread.h>

struct event_loop_context {



    int pipe_fd[2];
    pthread_mutex_t mtx;
    pthread_t thread_ctx;
};


static void thread_loop() {
}


void *event_loop_create() {
}

int event_loop_run(void *instance) {
    return 0;
}

void event_loop_destroy() {
}