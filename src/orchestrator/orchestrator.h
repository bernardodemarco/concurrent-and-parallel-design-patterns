#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

    typedef struct {
        pthread_t thread_id;
    } Orchestrator;

    void init_orchestrator();

#endif