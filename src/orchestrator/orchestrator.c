#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "./../globals.h"
#include "./../utils/utils.h"
#include "./../data-structures/hashmap/hashmap.h"

typedef struct {
    int actuator;
    int activity_level;
} UpdateActuatorArgs;

int has_failed() {
    return (rand() % 5) == 0;
}

int print_output(UpdateActuatorArgs *actuator_args) {
    int actuator = actuator_args -> actuator; 
    int activity_level = actuator_args -> activity_level;

    printf("Changing %d with value %d\n", actuator, activity_level);
    sleep(1);

    // int err = has_failed();
    // printf("exiting print_output(%d) | has_failed() = %d\n", actuator, err);
    return has_failed();
}

void *update_actuador(void *args) {
    UpdateActuatorArgs *actuator_args = (UpdateActuatorArgs *) args;

    int actuator = actuator_args -> actuator; 
    int activity_level = actuator_args -> activity_level;

    int time_to_hold = ((rand() % 1001) + 2000) / 1000;

    // printf("trying to go through (%d) the mutex...\n", actuator);
    pthread_mutex_lock(&(orchestrator -> hash_map_mutex));
    orchestrator -> hash_map -> add_value(orchestrator -> hash_map -> table, actuator, activity_level);
    // printf("before sleep (%d) inside of mutex...\n", actuator);
    sleep(time_to_hold);
    // printf("after sleep (%d) inside of mutex...\n", actuator);
    pthread_mutex_unlock(&(orchestrator -> hash_map_mutex));

    int err = has_failed();
    // printf("exiting update_actuator(%d) | has_failed() = %d\n", actuator, err);
    pthread_exit(&err);
}

// test creating thread to print_output

void manage_actuators(void *args) {
    UpdateActuatorArgs update_actuator_args;
    
    pthread_t actuator_thread_id;

    int *update_actuator_err;
    int print_output_err;

    int *captured_value_pointer = (int *) args;
    int captured_value = *captured_value_pointer;

    int actuator = captured_value % orchestrator -> num_of_actuators;
    // printf("recently generated actuator %d, captured value %d, num actuators %d\n", actuator, captured_value, orchestrator -> num_of_actuators);
    int activity_level = rand() % 101;

    update_actuator_args.actuator = actuator;
    update_actuator_args.activity_level = activity_level;

    pthread_create(&actuator_thread_id, NULL, update_actuador, (void *) (&update_actuator_args));

    print_output_err = print_output(&update_actuator_args);

    pthread_join(actuator_thread_id, (void *) &update_actuator_err);
    // printf("threads must be syncronized!\n");
    // printf("update_actuator_err = %d\n", *update_actuator_err);
    // printf("print_output_err = %d\n", print_output_err);
    if (*update_actuator_err || print_output_err) {
        printf("Fail: %d\n", actuator);
    }

    free(captured_value_pointer);
}

void *orchestrator_thread(void *args) {
    orchestrator -> thread_pool = thpool_init(4);

    while (1) {
        // remove this sleep
        // sleep(2);

        // need to control producer consumer buffer size with semaphore

        pthread_mutex_lock(&producer_consumer_mutex);
        int captured_value = queue -> dequeue();
        pthread_mutex_unlock(&producer_consumer_mutex);

        if (captured_value == -1) {
            // printf("captured -1, continueing\n");
            continue;
        }

        // remove print
        // printf("Received %d from the sensors\n", captured_value);

        int *captured_value_copy = (int *) malloc(sizeof(int)); 
        *captured_value_copy = captured_value;
        thpool_add_work(orchestrator -> thread_pool, (void *) manage_actuators, (void *) captured_value_copy);
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
