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