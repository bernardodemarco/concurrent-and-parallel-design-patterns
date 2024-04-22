#include <stdio.h>
#include <unistd.h>
#include "./globals.h"
#include "./sensor/sensor.h"
#include "./producer-consumer/queue.h"

int main() {
    printf("Welcome to the Autonomous Cars Parallel Application!\n");

    init_sensors();

    sleep(10);
    kill_sensors();
    return 0;
}