#include <stdio.h>
#include <unistd.h>
#include "./globals.h"
#include "./sensor/sensor.h"
#include "./orchestrator/orchestrator.h"
#include "./data-structures/queue/queue.h"

int main() {
    printf("Welcome to the Autonomous Cars Parallel Application!\n");

    init_queue();
    init_sensors();
    init_orchestrator();

    // sleep(10);
    // queue -> enqueue(10);
    // kill_sensors();
    return 0;
}