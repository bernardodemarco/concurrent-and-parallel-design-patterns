#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "./../globals.h"
#include "./../utils/utils.h"
#include "./../data-structures/hashmap/hashmap.h"
// #include "./../lib/thread-pool/thpool.h"

typedef struct {
    int actuator;
    int activity_level;
    pthread_barrier_t *syncronize_actuator_tasks;
} UpdateActuatorArgs;

bool has_failed() {
    return (rand() % 5) == 0;
}

void print_output(UpdateActuatorArgs *actuator_args) {
    pthread_barrier_t *barrier = actuator_args -> syncronize_actuator_tasks;
    int actuator = actuator_args -> actuator; 
    int activity_level = actuator_args -> activity_level;

    printf("Changing %d with value %d\n", actuator, activity_level);
    sleep(1);
    pthread_barrier_wait(barrier);
    printf("print output waiting in the barrier\n");
}

void *update_actuador(void *args) {
    UpdateActuatorArgs *actuator_args = (UpdateActuatorArgs *) args;

    pthread_barrier_t *barrier = actuator_args -> syncronize_actuator_tasks;
    int actuator = actuator_args -> actuator; 
    int activity_level = actuator_args -> activity_level;

    int time_to_hold = ((rand() % 1001) + 2000) / 1000;

    pthread_mutex_lock(&(orchestrator -> hash_map_mutex));
    orchestrator -> hash_map -> add_value(orchestrator -> hash_map -> table, actuator, activity_level);
    sleep(time_to_hold);
    pthread_mutex_unlock(&(orchestrator -> hash_map_mutex));

    pthread_barrier_wait(barrier);
    printf("update actuator waiting in the barrier\n");
}

void manage_actuators(void *args) {
    UpdateActuatorArgs update_actuator_args;
    
    pthread_t actuator_thread_id;
    
    pthread_barrier_t syncronize_actuator_tasks;
    pthread_barrier_init(&syncronize_actuator_tasks, NULL, 2);

    int captured_value = *((int *) args);
    printf("Received %d from the sensors\n", captured_value);

    int actuator = captured_value % orchestrator -> num_of_actuators;
    int activity_level = rand() % 101;

    update_actuator_args.syncronize_actuator_tasks = &syncronize_actuator_tasks;
    update_actuator_args.actuator = actuator;
    update_actuator_args.activity_level = activity_level;

// change this thread id
    pthread_create(&actuator_thread_id, NULL, update_actuador, (void *) (&update_actuator_args));

    print_output(&update_actuator_args);
    // WAIT IN THE BARRIER
    // MOVE ON

    printf("threads must be syncronized!\n");

    pthread_barrier_destroy(&syncronize_actuator_tasks);
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