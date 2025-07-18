#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H
    #include "./../lib/C-Thread-Pool/thpool.h"
    #include "./../data-structures/hashmap/hashmap.h"

    typedef struct {
        pthread_t thread_id;
        threadpool thread_pool;
        int num_of_actuators;
        pthread_mutex_t *hash_map_mutexes;
        int num_of_critical_sections;
        pthread_mutex_t console_mutex;
        HashMap* hash_map;
    } Orchestrator;

    void init_orchestrator();
    void syncronize_orchestrator();

#endif
