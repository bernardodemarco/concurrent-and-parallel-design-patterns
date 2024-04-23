#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "./../globals.h"
// #include "./../lib/thread-pool/thpool.h"

void *orchestrator_thread(void *args) {
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
        // thpool_add_work(orchestrator -> thread_pool);
    }
}

void init_orchestrator() {
    orchestrator = (Orchestrator *) malloc(sizeof(Orchestrator));
    orchestrator -> thread_pool = thpool_init(4);
    pthread_create(&(orchestrator -> thread_id), NULL, orchestrator_thread, NULL);
}

void syncronize_orchestrator() {
    int err = pthread_join(orchestrator -> thread_id, NULL);
    if (err) {
        printf("Error syncronizing orchestrator thread\n");
        exit(1);
    }
}