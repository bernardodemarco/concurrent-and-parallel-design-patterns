#include <stdio.h>
#include "./sensor/sensor.h"

int main() {
    printf("Welcome to the Autonomous Cars Parallel Application!\n");

    init_sensors();
    syncronize_threads();

    printf("THREADS MUST BE SYNCRONIZED...\n");

    return 0;
}