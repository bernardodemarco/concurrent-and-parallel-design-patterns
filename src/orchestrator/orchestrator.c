#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "./../globals.h"
#include "./../utils/utils.h"
// #include "./../lib/thread-pool/thpool.h"

void *update_actuador(void *args) {
    int actuator = ((int *) args)[0]; 
    int activity_level = ((int *) args)[1];

    // update hash map
    // KEY -> ACTUATOR
    // VALUE -> ACTIVITY LEVEL
    // HOLD FOR [2s, 3s]
    // THEN WAIT IN THE BARRIER
    // AFTER WAITING IN THE BARRIER -> EXIT
}

void manage_actuators(void *args) {
    int captured_value = *((int *) args);

    pthread_barrier_init(&orchestrator -> actuator_barrier, NULL, 2);
    printf("Received %d from the sensors\n", captured_value);

    int actuator = captured_value % orchestrator -> num_of_actuators;
    int activity_level = rand() % 101;

    int *update_actuator_args = (int *) malloc(2 * sizeof(int));
    update_actuator_args[0] = actuator;
    update_actuator_args[1] = activity_level;

    pthread_create(&(orchestrator -> actuator_thread_id), NULL, update_actuador, (void *) update_actuator_args);

    // PRINT VALUES TO THE CONSOLE
    // Alterando: <atuador> com valor <nível de atividade>\n
    // HOLD FOR 1 SEC
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

        // printf("Received %d from the sensors\n", captured_value);
        thpool_add_work(orchestrator -> thread_pool, (void *) manage_actuators, (void *) &captured_value);
    }
}

void init_orchestrator() {
    orchestrator = (Orchestrator *) malloc(sizeof(Orchestrator));
    orchestrator -> num_of_actuators = get_int_input("Enter the number of actuators of the vehicle: \n");
    pthread_create(&(orchestrator -> thread_id), NULL, orchestrator_thread, NULL);
}

void syncronize_orchestrator() {
    int err = pthread_join(orchestrator -> thread_id, NULL);
    if (err) {
        printf("Error syncronizing orchestrator thread\n");
        exit(1);
    }
}