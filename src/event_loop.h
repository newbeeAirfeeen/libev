//
// Created by 沈昊 on 2022/12/5.
//

#ifndef LIBEV_EVENT_LOOP_H
#define LIBEV_EVENT_LOOP_H
#include "ev.h"
#ifdef __cplusplus
extern "C" {
#endif

void *event_loop_create();
int event_loop_run(void *instance);
int event_loop_stop(void *instance);
int event_loop_async(void *instance, void (*p)(void *loop, void *args), void *args);
void event_loop_destroy(void *instance);

#ifdef __cplusplus
};
#endif
#endif//LIBEV_EVENT_LOOP_H
