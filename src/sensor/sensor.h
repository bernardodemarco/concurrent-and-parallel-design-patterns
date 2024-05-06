#ifndef SENSOR_H
#define SENSOR_H

    typedef struct {
        int num_of_sensors;
        pthread_t *threads_ids;
    } SensorsController;

    void *sensor_thread(void* args);
    void init_sensors();
    void syncronize_sensors();
    void kill_sensors();

#endif
