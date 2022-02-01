#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

//#include <duma.h>

//CONSTANT
#define TXT_LEN 256
#define HEAP_ELEM 100


//TYPDEFS
typedef struct entry_s {
    char songtitle[TXT_LEN], interpreter[TXT_LEN];
}entry_t;

typedef struct heap_s {
    entry_t *entries;
    long size;
    long next_free;
}heap_t;


typedef struct hasharray_s {
    entry_t *entries;
    long num_entries;
}hasharray_t;

typedef struct hash_s {
    hasharray_t *hasharrays;
    long hashsize;
}hash_t;

//HELPER
long readValue(char *name);
void my_getline(char *destination, long len, char *name);
void print_entry(heap_t *heap, long elem);
void up_heap(heap_t *heap, long size);
void down_heap(heap_t *heap, long size);
long hash_key (char songtitle[], char interpreter[], long hashsize);
//FUNCTIONS HEAP
heap_t *heap_create(long size);
long heap_insert(heap_t* heap, char songtitle[], char interpreter[]);
void heap_print(heap_t *heap);
void heap_free(heap_t *heap);
long heap_search(heap_t *heap, char songtitle[], char interpreter[]);
long heap_delete_first(heap_t *heap);
//FUNCTIONS HASH
hash_t *hash_create(long hashsize);
void hash_insert(hash_t *hash, char songtitle[], char interpreter[]);
void hash_print(hash_t *hash);
void hash_free(hash_t *hash);
hash_t *convert_heap_to_hash(heap_t *heap);
entry_t *hash_search(hash_t *hash, char songtitle[], char interpreter[]);
long hash_delete_entry(hash_t * hash, char songtitle[], char interpreter[]);
void benchmark();
//FUNCTIONS STRING GENERATION
char *get_random_string(size_t length);
char **generate_random_string_array(size_t array_size, size_t string_length);
void free_string_array(char **string_array, size_t array_size);

#endif // FUNCTIONS_H_INCLUDED
