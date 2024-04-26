#ifndef HASHMAP_H
#define HASHMAP_H

    typedef struct {
        int *table;
        void (* add_value)(int *, int, int);
        int (* get_value)(int *, int);
    } HashMap;

    HashMap *init_hash_map(int num_of_fields);

#endif