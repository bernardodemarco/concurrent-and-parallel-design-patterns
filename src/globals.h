#ifndef GLOBALS_H
#define GLOBALS_H
    #include <pthread.h>

    typedef struct {
        int num_of_sensors;
        pthread_t *threads_ids;
        int *ids;
    } SensorsController;

    extern SensorsController *sensors_controller;

#endif