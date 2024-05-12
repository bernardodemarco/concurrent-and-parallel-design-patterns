#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "./../utils/utils.h"
#include "./../globals/globals.h"

void *sensor_thread(void *args) {
    while (1) {
        int task_duration_time = rand() % 5 + 1;
        sleep(task_duration_time);

        int captured_value = rand() % 1001;
        
        pthread_mutex_lock(&producer_consumer_mutex);
        queue -> enqueue(captured_value);
        pthread_mutex_unlock(&producer_consumer_mutex);
    }
}

void init_sensors(int num_of_sensors) {
    sensors_controller.num_of_sensors = num_of_sensors;     
    sensors_controller.threads_ids = (pthread_t *) malloc(sensors_controller.num_of_sensors * sizeof(pthread_t));

    for (int i = 0; i < sensors_controller.num_of_sensors; i++) {
        int err = pthread_create(&(sensors_controller.threads_ids[i]), NULL, &sensor_thread, NULL);
        if (err) {
            printf("Error creating sensor thread %d\n", i);
            exit(1);
        }
    }
}

void syncronize_sensors() {
    for (int i = 0; i < sensors_controller.num_of_sensors; i++) {
        int err = pthread_join(sensors_controller.threads_ids[i], NULL);
        if (err) {
            printf("Error syncronizying sensor thread %d\n", i);
            exit(1);
        }
    }
}

void kill_threads() {
    for (int i = 0; i < sensors_controller.num_of_sensors; i++) {
        pthread_cancel(sensors_controller.threads_ids[i]);
    }
}

void free_sensors() {
    free(sensors_controller.threads_ids);
}

void kill_sensors() {
    kill_threads();
    free_sensors();
}
