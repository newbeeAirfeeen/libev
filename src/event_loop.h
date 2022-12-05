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
void event_loop_destroy();

#ifdef __cplusplus
};
#endif
#endif//LIBEV_EVENT_LOOP_H
