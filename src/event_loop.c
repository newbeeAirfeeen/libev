//
// Created by 沈昊 on 2022/12/5.
//
#include "event_loop.h"
#include "list.h"
#include "socket_ops.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
extern int errno;
struct task_node {
    void (*p)(void *loop, void *args);
    void *args;
};

void node_free(void *node) {
    if (!node) {
        return;
    }
    LIST_FREE(node);
    node = NULL;
}


struct event_loop_context {
    int pipe_fd[2];
    pthread_mutex_t mtx;
    pthread_cond_t run_flag;
    list_t *task_queue;
    struct ev_loop *loop;
    pthread_t thread_ctx;
    ev_io pipe_io;
};

static void pipe_event(struct ev_loop *loop, struct ev_io *io, int events) {
    struct event_loop_context *ctx = (struct event_loop_context *) ev_userdata(loop);
    list_t *tmp_task_queue = NULL;
    pthread_mutex_lock(&ctx->mtx);
    tmp_task_queue = ctx->task_queue;
    ctx->task_queue = list_new();
    //ctx->task_queue_not_empty = 0;
    pthread_mutex_unlock(&ctx->mtx);

    char ch;
    while(1){
        int ret = read(ctx->pipe_fd[0], &ch, 1);
        if( ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)){
            break;
        }
    }

    if (!tmp_task_queue) {
        return;
    }

    list_iterator_t *top = list_iterator_new(tmp_task_queue, LIST_HEAD);
    list_iterator_t *iterator = top;
    list_node_t *node = NULL;
    while ((node = list_iterator_next(iterator)) != NULL) {
        struct task_node *task_node_ = (struct task_node *) node->val;
        void (*func)(void *, void *) = (void (*)(void *, void *)) task_node_->p;
        if (func) {
            func(ctx, task_node_->args);
        }
    }
    if (top) {
        list_iterator_destroy(top);
    }
}

static void thread_loop_exit(void *instance, void *args) {
    if (!instance) {
        return;
    }
    struct event_loop_context *ctx = (struct event_loop_context *) args;
    if (!ctx->loop) {
        return;
    }
    event_loop_stop(ctx->loop);
}
/// thread_loop
static void *thread_loop(void *args) {
    struct event_loop_context *ctx = (struct event_loop_context *) args;
    if (!ctx) {
        goto _exit;
    }
    /// 通知已经启动
    pthread_cond_signal(&ctx->run_flag);
    //    while (1) {
    //        pthread_mutex_lock(&ctx->mtx);
    //        while (!ctx->quit_ && !ctx->task_queue_not_empty) {
    //            pthread_cond_wait(&ctx->run_flag, NULL);
    //        }
    //        /// 如果设置退出
    //        if (ctx->quit_) {
    //            pthread_mutex_unlock(&ctx->mtx);
    //            goto _exit;
    //        }
    //        pthread_mutex_unlock(&ctx->mtx);
    ev_io_start(ctx->loop, &ctx->pipe_io);
    ev_run(ctx->loop, 0);
//    }
_exit:
    pthread_exit(NULL);
}


void *event_loop_create() {
    /// 创建管道
    int pipe_arr[2] = {-1, -1};
    int ret = pipe(pipe_arr);
    if (ret < 0) {
        goto failed;
    }

    ret = make_fd_nonblock(pipe_arr[0], 1);
    if (ret != 0) {
        goto failed;
    }

    ret = make_fd_nonblock(pipe_arr[1], 1);
    if (ret != 0) {
        goto failed;
    }

    struct event_loop_context *ctx = malloc(sizeof(struct event_loop_context));
    memset(ctx, 0, sizeof(struct event_loop_context));

    /// pipe fd
    ctx->pipe_fd[0] = pipe_arr[0];
    ctx->pipe_fd[1] = pipe_arr[1];

    pthread_cond_init(&ctx->run_flag, NULL);
    pthread_mutex_init(&ctx->mtx, NULL);
    ctx->task_queue = list_new();
    ctx->task_queue->free = node_free;
    ctx->loop = ev_default_loop(0);
    /// 初始化管道io
    ev_io_init(&ctx->pipe_io, pipe_event, ctx->pipe_fd[0], EV_READ);
    /// 设置上下文
    ev_set_userdata(ctx->loop, ctx);
    return ctx;
failed:
    if (pipe_arr[0]) {
        close(pipe_arr[0]);
    }
    if (pipe_arr[1]) {
        close(pipe_arr[1]);
    }
    return NULL;
}

int event_loop_run(void *instance) {
    struct event_loop_context *ctx = (struct event_loop_context *) instance;
    if (!ctx) {
        return -1;
    }
    pthread_create(&ctx->thread_ctx, NULL, thread_loop, ctx);
    {
        /// 等待启动
        pthread_mutex_lock(&ctx->mtx);
        pthread_cond_wait(&ctx->run_flag, &ctx->mtx);
        pthread_mutex_unlock(&ctx->mtx);
    }
    return 0;
}

int event_loop_stop(void *instance) {
    if (!instance) {
        return -1;
    }
    struct event_loop_context *ctx = (struct event_loop_context *) instance;
    event_loop_async(instance, thread_loop_exit, NULL);
    pthread_join(ctx->thread_ctx, NULL);
    return 0;
}

int event_loop_async(void *instance, void (*p)(void *loop, void *args), void *args) {
    if (!instance || !p) {
        return -1;
    }
    struct event_loop_context *ctx = (struct event_loop_context *) instance;
    struct task_node *node = LIST_MALLOC(sizeof(struct task_node));
    node->p = p;
    node->args = args;
    pthread_mutex_lock(&ctx->mtx);
    list_rpush(ctx->task_queue, list_node_new(node));
    pthread_mutex_unlock(&ctx->mtx);
    write(ctx->pipe_fd[1], "1", 1);
    return 0;
}

void event_loop_destroy(void *instance) {
    if (event_loop_stop(instance) != 0) {
        return;
    }
    struct event_loop_context *ctx = (struct event_loop_context *) instance;

    if (ctx->pipe_fd[0] > 0) {
        close(ctx->pipe_fd[0]);
    }

    if (ctx->pipe_fd[1]) {
        close(ctx->pipe_fd[1]);
    }

    pthread_cond_destroy(&ctx->run_flag);
    pthread_mutex_destroy(&ctx->mtx);

    if (ctx->task_queue) {
        list_destroy(ctx->task_queue);
    }


    if (ctx->loop) {
        ev_loop_destroy(ctx->loop);
    }

    free(ctx);
}
