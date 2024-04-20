#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "./../utils/utils.h"


int NUM_SENSORS;
pthread_t *sensors_threads_ids;
int *sensors_ids;

void *sensor_thread(void *args) {
    int id = *((int *) args);
    printf("Sensor thread id = %d\n", id);

    pthread_exit(NULL);
}

void init_sensors() {
    NUM_SENSORS = get_int_input("Enter the number of sensors of the vehicle: \n");     
    sensors_threads_ids = (pthread_t *) malloc(NUM_SENSORS * sizeof(pthread_t));
    sensors_ids = (int *) malloc(NUM_SENSORS * sizeof(int));

    for (int i = 0; i < NUM_SENSORS; i++) {
        sensors_ids[i] = i;
        int err = pthread_create(&(sensors_threads_ids[i]), NULL, &sensor_thread, &(sensors_ids[i]));
        if (err) {
            printf("Error creating thread!\n");
            exit(1);
        }
    }
}

void syncronize_threads() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        int err = pthread_join(sensors_threads_ids[i], NULL);
        if (err) {
            printf("Error sincronizing threads!");
            exit(1);
        }
    }
}

