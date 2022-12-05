//
// Created by 沈昊 on 2022/12/2.
//
#include <stdio.h>
#include <ev.h>
#include <event_loop.h>
void timer_action(struct ev_loop *main_loop,ev_timer *timer_w,int e)
{
    printf("hello,world\n");
}
int main(){

    ev_io io;
    ev_timer timer;



    struct ev_loop* loop = ev_default_loop(0);
    //ev_timer_init(&timer, timer_action, 1, 1);
    //ev_timer_start(loop, &timer);
    ev_t
    ev_run(loop, 0);
    return 0;
}