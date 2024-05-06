#include "./globals.h"

SensorsController *sensors_controller;
Queue *queue;
Orchestrator *orchestrator;
pthread_mutex_t producer_consumer_mutex;
