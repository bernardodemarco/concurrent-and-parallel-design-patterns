#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "./../globals/globals.h"
#include "./../utils/utils.h"
#include "./../data-structures/hashmap/hashmap.h"

typedef struct {
    int mapping[2];
    int *update_actuator_err;
} UpdateActuatorArgs;

int has_failed() {
    return (rand() % 5) == 0;
}

int print_output(UpdateActuatorArgs actuator_args) {
    int actuator = actuator_args.mapping[0]; 
    int activity_level = actuator_args.mapping[1];

    pthread_mutex_lock(&(orchestrator.console_mutex));
    printf("Changing %d with value %d\n", actuator, activity_level);
    sleep(1);
    pthread_mutex_unlock(&(orchestrator.console_mutex));

    return has_failed();
}

void *update_actuador(void *args) {
    UpdateActuatorArgs *actuator_args = (UpdateActuatorArgs *) args;

    int actuator = actuator_args -> mapping[0]; 
    int activity_level = actuator_args -> mapping[1];

    int time_to_hold = (rand() % 2) + 2;

    int actuator_critical_section = actuator % orchestrator.num_of_critical_sections;
    
    printf("actuator = %d && num_of_critical_sections = %d && critical_section = %d\n", actuator, orchestrator.num_of_critical_sections, actuator_critical_section);
    
    pthread_mutex_lock(&(orchestrator.hash_map_mutexes[actuator_critical_section]));
    orchestrator.hash_map -> add_value(orchestrator.hash_map -> table, actuator, activity_level);
    sleep(time_to_hold);
    pthread_mutex_unlock(&(orchestrator.hash_map_mutexes[actuator_critical_section]));

    *(actuator_args -> update_actuator_err) = has_failed();
}

void manage_actuators(void *args) {
    pthread_t actuator_thread_id;

    UpdateActuatorArgs update_actuator_args;
    
    int *update_actuator_err = (int *) malloc(sizeof(int));
    int print_output_err;

    int *captured_value_pointer = (int *) args;
    int captured_value = *captured_value_pointer;

    int actuator = captured_value % orchestrator.num_of_actuators;
    int activity_level = rand() % 101;

    update_actuator_args.mapping[0] = actuator;
    update_actuator_args.mapping[1] = activity_level;
    update_actuator_args.update_actuator_err = update_actuator_err;

    pthread_create(&actuator_thread_id, NULL, update_actuador, (void *) (&(update_actuator_args)));

    print_output_err = print_output(update_actuator_args);

    pthread_join(actuator_thread_id, NULL);
    if (*update_actuator_err || print_output_err) {
        printf("\033[0;31mFail: %d\n\033[0m", actuator);
    }

    free(captured_value_pointer);
}

void *orchestrator_thread(void *args) {
    orchestrator.thread_pool = thpool_init(4);

    while (1) {
        pthread_mutex_lock(&producer_consumer_mutex);
        int captured_value = queue -> dequeue();
        pthread_mutex_unlock(&producer_consumer_mutex);

        if (captured_value == -1) {
            continue;
        }

        int *captured_value_copy = (int *) malloc(sizeof(int)); 
        *captured_value_copy = captured_value;
        thpool_add_work(orchestrator.thread_pool, (void *) manage_actuators, (void *) captured_value_copy);
    }
}

int get_number_of_hash_map_critical_sections(double num_of_fields) {
    int granularity = 20;

    return (int) ceil(num_of_fields / granularity);
}

void init_hash_map_mutexes(int num_of_fields) {
    int num_of_sections = get_number_of_hash_map_critical_sections((double) num_of_fields);
    printf("(num_of_sections = %d) \n", num_of_sections);

    pthread_mutex_t *mutexes = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * num_of_sections);
    for (int i = 0; i < num_of_sections; i++) {
        pthread_mutex_init(&(mutexes[i]), NULL);
    }

    orchestrator.hash_map_mutexes = mutexes;
    orchestrator.num_of_critical_sections = num_of_sections;
}

void init_orchestrator(int num_of_actuators) {
    orchestrator.num_of_actuators = num_of_actuators;
    orchestrator.hash_map = init_hash_map(orchestrator.num_of_actuators);

    init_hash_map_mutexes(num_of_actuators);
    pthread_mutex_init(&(orchestrator.console_mutex), NULL);

    pthread_create(&(orchestrator.thread_id), NULL, orchestrator_thread, NULL);
}

void syncronize_orchestrator() {
    int err = pthread_join(orchestrator.thread_id, NULL);
    if (err) {
        printf("Error syncronizing orchestrator thread\n");
        exit(1);
    }
}
