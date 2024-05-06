#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "./../utils/utils.h"
#include "./../globals.h"

void *sensor_thread(void *args) {
    while (true) {
        // generate in ms then convert to s
        // evaluate usage of seeds
        int task_duration_time = rand() % 5 + 1;
        sleep(task_duration_time);

        int captured_value = rand() % 1001;
        
        pthread_mutex_lock(&producer_consumer_mutex);
        queue -> enqueue(captured_value);
        pthread_mutex_unlock(&producer_consumer_mutex);
    }
}

void init_sensors(int num_of_sensors) {
    sensors_controller = (SensorsController *) malloc(sizeof(SensorsController));
    sensors_controller -> num_of_sensors = num_of_sensors;     
    sensors_controller -> threads_ids = (pthread_t *) malloc(sensors_controller -> num_of_sensors * sizeof(pthread_t));
    sensors_controller -> ids = (int *) malloc(sensors_controller -> num_of_sensors * sizeof(int));

    for (int i = 0; i < sensors_controller -> num_of_sensors; i++) {
        sensors_controller -> ids[i] = i;
        int err = pthread_create(&(sensors_controller -> threads_ids[i]), NULL, &sensor_thread, &(sensors_controller -> ids[i]));
        if (err) {
            printf("Error creating sensor thread %d\n", i);
            exit(1);
        }
    }
}

void syncronize_sensors() {
    for (int i = 0; i < sensors_controller -> num_of_sensors; i++) {
        int err = pthread_join(sensors_controller -> threads_ids[i], NULL);
        if (err) {
            printf("Error syncronizying sensor thread %d\n", i);
            exit(1);
        }
    }
}

void kill_threads() {
    for (int i = 0; i < sensors_controller -> num_of_sensors; i++) {
        pthread_cancel(sensors_controller -> threads_ids[i]);
    }
}

void free_sensors() {
    free(sensors_controller -> ids);
    free(sensors_controller -> threads_ids);
    free(sensors_controller);
}

void kill_sensors() {
    kill_threads();
    free_sensors();
}
