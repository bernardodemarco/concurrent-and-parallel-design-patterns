#ifndef QUEUE_H
#define QUEUE_H
    typedef struct Node {
        int value;
        struct Node *next;
    } Node;

    typedef struct {
        Node *head;
        Node *tail;
        void (* enqueue)(int);
        int (* dequeue)();
    } Queue;

    void init_queue();
    void free_queue();
#endif
