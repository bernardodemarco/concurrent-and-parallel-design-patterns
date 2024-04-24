#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H
    #include "./../lib/C-Thread-Pool/thpool.h"

    typedef struct {
        pthread_t thread_id;
        threadpool thread_pool;
    } Orchestrator;

    void init_orchestrator();
    void syncronize_orchestrator();

#endif