#ifndef GLOBALS_H
#define GLOBALS_H

    #include <pthread.h>
    #include "./producer-consumer/queue.h"
    #include "./sensor/sensor.h"

    extern SensorsController *sensors_controller;
    extern Queue *queue;

#endif