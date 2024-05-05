#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "./../globals.h"
#include "./../utils/utils.h"
#include "./../data-structures/hashmap/hashmap.h"
// #include "./../lib/thread-pool/thpool.h"

void print_output(int *actuator_args) {
    printf("Changing %d with value %d\n", actuator_args[0], actuator_args[1]);
    sleep(1);
    pthread_barrier_wait(&(orchestrator -> actuator_barrier));
}

void *update_actuador(void *args) {
    int actuator = ((int *) args)[0]; 
    int activity_level = ((int *) args)[1];

    int time_to_hold = ((rand() % 1001) + 2000) / 1000;

    pthread_mutex_lock(&(orchestrator -> hash_map_mutex));
    orchestrator -> hash_map -> add_value(orchestrator -> hash_map -> table, actuator, activity_level);
    sleep(time_to_hold);
    pthread_mutex_unlock(&(orchestrator -> hash_map_mutex));

    pthread_barrier_wait(&(orchestrator -> actuator_barrier));
}

void manage_actuators(void *args) {
    int update_actuator_args[2];
    int captured_value = *((int *) args);

// check the scope of this barrier
    pthread_barrier_init(&orchestrator -> actuator_barrier, NULL, 2);
    printf("Received %d from the sensors\n", captured_value);

    int actuator = captured_value % orchestrator -> num_of_actuators;
    int activity_level = rand() % 101;

    update_actuator_args[0] = actuator;
    update_actuator_args[1] = activity_level;

    pthread_create(&(orchestrator -> actuator_thread_id), NULL, update_actuador, (void *) (&update_actuator_args));

    print_output(update_actuator_args);
    // WAIT IN THE BARRIER
    // MOVE ON
}

void *orchestrator_thread(void *args) {
    orchestrator -> thread_pool = thpool_init(4);

    while (true) {
        // printf("I'll consume the bufffer!\n");
        sleep(2);

        // consume buffer
        pthread_mutex_lock(&producer_consumer_mutex);
        int captured_value = queue -> dequeue();
        pthread_mutex_unlock(&producer_consumer_mutex);

        if (captured_value == -1) {
            continue;
        }

        printf("Received %d from the sensors\n", captured_value);
        thpool_add_work(orchestrator -> thread_pool, (void *) manage_actuators, (void *) &captured_value);
    }
}

void init_orchestrator(int num_of_actuators) {
    orchestrator = (Orchestrator *) malloc(sizeof(Orchestrator));
    orchestrator -> num_of_actuators = num_of_actuators;
    orchestrator -> hash_map = init_hash_map(orchestrator -> num_of_actuators);
    pthread_mutex_init(&(orchestrator -> hash_map_mutex), NULL);
    pthread_create(&(orchestrator -> thread_id), NULL, orchestrator_thread, NULL);
}

void syncronize_orchestrator() {
    int err = pthread_join(orchestrator -> thread_id, NULL);
    if (err) {
        printf("Error syncronizing orchestrator thread\n");
        exit(1);
    }
}