#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "./../globals.h"

void *orchestrator_thread(void *args) {
    while (true) {
        printf("I'll consume the bufffer!\n");
        sleep(1);
        // consume buffer
    }
}

void init_orchestrator() {
    orchestrator = (Orchestrator *) malloc(sizeof(Orchestrator));
    pthread_create(&(orchestrator -> thread_id), NULL, orchestrator_thread, NULL);
}