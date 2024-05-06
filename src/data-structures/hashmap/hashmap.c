#include <stdlib.h>

#include "./hashmap.h"

void add_value(int *table, int bucket_index, int value) {
    table[bucket_index] = value;
}

int get_value(int *table, int bucket_index) {
    return table[bucket_index];
}

HashMap *init_hash_map(int num_of_fields) {
    HashMap *hash_map = (HashMap *) malloc(sizeof(HashMap));
    hash_map -> table = (int *) malloc(num_of_fields * sizeof(int));
    hash_map -> add_value = add_value;
    hash_map -> get_value = get_value;
    return hash_map;
}
