#ifndef GLOBALS_H
#define GLOBALS_H

    #include <pthread.h>

    #include "./../data-structures/queue/queue.h"
    #include "./../sensor/sensor.h"
    #include "./../orchestrator/orchestrator.h"

    extern SensorsController sensors_controller;
    extern Queue *queue;
    extern Orchestrator orchestrator;
    extern pthread_mutex_t producer_consumer_mutex;

#endif
