#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "./globals/globals.h"
#include "./sensor/sensor.h"
#include "./orchestrator/orchestrator.h"
#include "./data-structures/queue/queue.h"
#include "./utils/utils.h"

typedef struct {
    int num_of_sensors;
    int num_of_actuators;
} Input;

void suspend_main_thread() {
    syncronize_sensors();
    syncronize_orchestrator();
}

Input get_inputs() {
    Input inputs;
    inputs.num_of_sensors = get_int_input("Enter the number of sensors of the vehicle:");
    inputs.num_of_actuators = get_int_input("Enter the number of actuators of the vehicle:");
    return inputs;
}

int main() {
    printf("\033[0;32mWelcome to the Autonomous Cars Parallel Application!\n\033[0m");

    Input inputs = get_inputs();

    pthread_mutex_init(&producer_consumer_mutex, NULL);

    init_queue();
    init_sensors(inputs.num_of_sensors);
    init_orchestrator(inputs.num_of_actuators);

    suspend_main_thread();
    pthread_mutex_destroy(&producer_consumer_mutex);
    return 0;
}
