#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "./globals.h"
#include "./sensor/sensor.h"
#include "./orchestrator/orchestrator.h"
#include "./data-structures/queue/queue.h"

void suspend_main_thread() {
    syncronize_sensors();
    syncronize_orchestrator();
}

int main() {
    printf("Welcome to the Autonomous Cars Parallel Application!\n");

    pthread_mutex_init(&producer_consumer_mutex, NULL);

    init_queue();
    init_sensors();
    init_orchestrator();

    suspend_main_thread();
    pthread_mutex_destroy(&producer_consumer_mutex);
    return 0;
}