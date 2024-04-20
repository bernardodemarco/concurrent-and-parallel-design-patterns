#ifndef SENSOR_H
#define SENSOR_H

    void *sensor_thread(void* args);
    void init_sensors();
    void syncronize_threads();

#endif