#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H
    #include "./../lib/C-Thread-Pool/thpool.h"

    typedef struct {
        pthread_t thread_id;
        threadpool thread_pool;
        int num_of_actuators;
        pthread_t actuator_thread_id;
        pthread_barrier_t actuator_barrier;
    } Orchestrator;

    void init_orchestrator();
    void syncronize_orchestrator();

#endif