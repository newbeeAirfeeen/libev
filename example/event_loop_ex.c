//
// Created by 沈昊 on 2022/12/5.
//

#include "event_loop.h"
#include "stdio.h"
void run(void *instance, void *args) {
    static int val = 1;
    printf("yes, it is %d\n", val++);
    event_loop_async(instance, run, NULL);
}
int main() {
    void *loop = event_loop_create();
    event_loop_run(loop);
    event_loop_async(loop, run, NULL);
    getchar();
    return 0;
}