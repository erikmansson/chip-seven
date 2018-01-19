#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <machine.h>
#include <window.h>
#include <helpers.h>

#define DECR_TIMER_FREQ 16
#define WINDOW_UPDATE_FREQ 60
#define EMUL_CYCLE_FREQ 1000

struct intrv_callee {
    void (*fun)(void);
    long intv_ns;
    struct timespec last_call;
};

struct intrv_callee intrv_callees[] = {
    {decr_timers, NS_IN_1_S/DECR_TIMER_FREQ, {}},
    {window_update, NS_IN_1_S/WINDOW_UPDATE_FREQ, {}},
    {perform_cycle, NS_IN_1_S/EMUL_CYCLE_FREQ, {}}
};

const size_t n_intrv_callees =
    sizeof(intrv_callees)/sizeof(struct intrv_callee);

int main(int argc, char *argv[])
{
    srand(time(NULL));

    printf("chip-seven\n");

    if(argc < 2){
        printf("error: no program file supplied\n");
        return 0;
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    if(file == NULL){
        printf("error: program file (%s) not found\n", filename);
        return 0;
    }

    size_t tmp_addr = PROG_START_ADDR;
    int c;
    while((c = getc(file)) != EOF){
        if(tmp_addr >= MEM_SIZE){
            printf("error: program file too long\n");
            return 0;
        }

        mem[tmp_addr++] = c;
    }

    fclose(file);

    init_machine();
    window_setup();

    while(!window_should_close()){
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);

        int should_sleep = 1;
        long min_dt_ns = LONG_MAX;
        for(size_t i = 0; i < n_intrv_callees; i++){
            struct intrv_callee *ic = &intrv_callees[i];
            long dt_ns = delta_time_ns(&ic->last_call, &now);

            if(dt_ns >= ic->intv_ns){
                ic->last_call = now;
                ic->fun();
                should_sleep = 0;
                continue;
            }

            if(dt_ns < min_dt_ns){
                min_dt_ns = dt_ns;
            }
        }

        if(should_sleep){
            struct timespec sleep_time = {
                min_dt_ns / NS_IN_1_S,
                min_dt_ns % NS_IN_1_S
            };

            nanosleep(&sleep_time, NULL);
        }
    }

    window_exit();

    return 0;
}
