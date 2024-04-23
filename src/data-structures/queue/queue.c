#include <stdlib.h>
#include <stdio.h>

#include "./queue.h"
#include "./../../globals.h"

void enqueue(int value) {
    Node *element_to_be_inserted = (Node *) malloc(sizeof(Node));
    
    element_to_be_inserted -> value = value;
    element_to_be_inserted -> next = (Node *) malloc(sizeof(Node));
    element_to_be_inserted -> next = NULL;

    if (queue -> head == NULL && queue -> tail == NULL) {
        queue -> head = element_to_be_inserted;
        queue -> tail = element_to_be_inserted;
        return;
    }

    queue -> tail -> next = element_to_be_inserted;
    queue -> tail = element_to_be_inserted;
}

void free_node(Node *node) {
    free(node);
}

int dequeue() {
    Node *element_to_be_removed = queue -> head;
    int value_to_be_returned = element_to_be_removed -> value;
    queue -> head = queue -> head -> next;

    if (queue -> head == NULL) {
        queue -> tail = NULL;
    }

    free_node(element_to_be_removed);
    return value_to_be_returned;
}

void init_queue() {
    queue = (Queue *) malloc(sizeof(Queue));
    queue -> head = (Node *) malloc(sizeof(Node));
    queue -> tail = (Node *) malloc(sizeof(Node));
    queue -> head = NULL;
    queue -> tail = NULL;
    queue -> enqueue = enqueue;
    queue -> dequeue = dequeue;
}

void free_queue() {
    free(queue);
}

void print_queue() {
    Node *iterator = (Node *) malloc(sizeof(Node));
    iterator = queue -> head;

    printf("START - ");
    while (iterator != NULL) {
        printf("%d - ", iterator -> value);
        iterator = iterator -> next;
    }
    printf("END\n");
}

int test_queue() {
    init_queue();
    
    queue -> enqueue(10);
    queue -> enqueue(20);
    queue -> enqueue(12);
    queue -> enqueue(22);
    queue -> enqueue(50);

    print_queue();

    queue -> dequeue();
    queue -> dequeue();

    print_queue();

    queue -> dequeue();
    queue -> dequeue();
    int last_removed_value = queue -> dequeue();
    printf("Last removed value = %d\n", last_removed_value);

    queue -> enqueue(42);
    queue -> enqueue(88);
    print_queue();

    free_queue();
}