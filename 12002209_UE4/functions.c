#include "functions.h"

long readValue(char *name) {
    /* Scans input for a long and return this value */
    long int_val = 0;
    int flag_error = 0;

    while(1) {
        fprintf(stdout, ">>%-*s: ", 10, name);
        flag_error = scanf("%ld", &int_val);
        fflush(stdin);

        if(flag_error == 1)
            break;
        fprintf(stderr, "ERROR: Invalid input for %s. \n", name);
    }
    return int_val;
}

void my_getline(char *destination, long len, char *name){
    /*Reads from stream stdin a string with certain length and stores it under specifies adress*/
    char input[len];
    long index = 0;
    char cur_char = ' ';

    memset(&input[0], 0, sizeof(input));
    fprintf(stdout, ">>%-*s: ", 10, name);
    for(index = 0; index+1 < len; index++){
        cur_char = tolower(getchar());
        if(cur_char == '\n')
            break;
        input[index] = cur_char;
    }
    input[index] = '\0';
    fflush(stdin);
    memcpy(destination, &input, len);
    return;
}

void up_heap(heap_t *heap, long size){
    entry_t *entries = heap->entries;
    entry_t new_entry;
    memcpy(&new_entry, &(entries[heap->next_free]), sizeof(entry_t));
    while ((size >= 2) && strcmp(entries[size/2].songtitle, new_entry.songtitle) < 0 )
    {
        memcpy(&(entries[size]),&(entries[size/2]), sizeof(entry_t));
        size /= 2;
    }
    memcpy(&(entries[size]), &new_entry, sizeof(entry_t));
}

void down_heap(heap_t *heap, long size){
    long left = 0, right = 0;
    long index = 1;

    entry_t *entries = heap->entries;
    entry_t new_entry;
    memcpy(&new_entry, &(entries[1]), sizeof(entry_t));

    while(left < size || right < size){
        left  = 2 * index;
        right = left + 1;
        if(right < size && strcmp(entries[right].songtitle, new_entry.songtitle) > 0){
            memcpy(&(entries[index]), &(entries[right]), sizeof(entry_t));
            index = right;
        }
        else if(left < size && strcmp(entries[2 * index].songtitle, new_entry.songtitle) > 0){
            memcpy(&(entries[index]), &(entries[left]), sizeof(entry_t));
            index = left;
        }
    }
     memcpy(&(entries[index]), &new_entry, sizeof(entry_t));

}

long hash_key (char songtitle[], char interpreter[], long hashsize){
    unsigned long index = 0, i;
    for(i = 0; i < strlen(songtitle); ++i)
        index = 64 * index + (long)(songtitle[i]);
    for(i = 0; i < strlen(interpreter); ++i)
        index = 64 * index + (long)(interpreter[i]);
    return index % hashsize;
}

//TASK A
heap_t *heap_create(long size) {
    heap_t *heap_ptr = NULL;
    entry_t *entries = NULL;

    if(!size)   return NULL;

    entries = calloc(size, sizeof(entry_t));
    heap_ptr = calloc(1, sizeof(heap_t));
    heap_ptr->size=size;
    heap_ptr->next_free = 1;
    heap_ptr->entries = entries;
    return heap_ptr;
}

long heap_insert(heap_t* heap, char songtitle[], char interpreter[]){
    if(!heap) return 0;

    if (heap->next_free < HEAP_ELEM){
        strcpy(heap->entries[heap->next_free].songtitle, songtitle);
        strcpy(heap->entries[heap->next_free].interpreter, interpreter);
        up_heap(heap, heap->next_free);
        heap->next_free++;
    }
    else
        return 0;
    return 1;
}

//TASK B
void heap_free(heap_t *heap) {
    if (!heap)  return;

    free(heap->entries);
    heap->entries = NULL;
    free(heap);
    heap = NULL;
}

void heap_print(heap_t *heap){
    long max_elem = heap->next_free -1;
    long max_level = log2(max_elem);
    long cur_elem = pow(2, max_level);
    long factor_max = cur_elem;
    long max_level_elem = cur_elem * 2 -1;
    long factor = 2, counter = 0;
    long j = 1;
    print_entry(heap, cur_elem);
    if (max_elem == 1)
        return;
    do {
        cur_elem = cur_elem / factor;
        if (cur_elem <= max_elem)
            print_entry(heap, cur_elem);
        cur_elem = cur_elem * factor + factor/2;
        j = 1;
        while(counter %(factor_max/j) != 0)
            j *= 2;
        counter += 2;
        factor = factor_max/j;
        if (cur_elem <= max_elem)
            print_entry(heap, cur_elem);
    }while(cur_elem != max_level_elem);
}

void print_entry(heap_t *heap, long elem){
    long spacing = log(elem)/log(2);
    printf("%ld.\t%*s - %s\n",elem, ((int)spacing*4), heap->entries[elem].songtitle, heap->entries[elem].interpreter);
}

//TASK C
long heap_search(heap_t *heap, char songtitle[], char interpreter[]){
    long index = 1;
    while(index < heap->next_free){
        if(strcmp(heap->entries[index].songtitle, songtitle) == 0){
            if(strcmp(heap->entries[index].interpreter, interpreter) == 0){
                return index;
            }
        }
        index++;
    }
    return 0;
}

//TASK D
long heap_delete_first(heap_t *heap){
    if(!heap)
        return 0;
    if(heap->next_free > 1){
        heap->next_free--;
        memcpy(&(heap->entries[1]), &(heap->entries[heap->next_free]), sizeof(entry_t));
        strcpy(heap->entries[heap->next_free].songtitle, "\0");
        strcpy(heap->entries[heap->next_free].interpreter , "\0");
        down_heap(heap, heap->next_free);
        if (heap->next_free != 1)
            return 1;
    }
    fprintf(stderr, "ERROR: Heap already empty");
    return 0;
}

//TASK E
hash_t *hash_create(long hashsize){
    hash_t *new_hash = NULL;
    hasharray_t *new_hash_array = NULL;

    new_hash = calloc(1, sizeof(hash_t));
    if (!new_hash){
        fprintf(stderr, "ERROR: Memory allocation for hash failed!\n");
        return NULL;
    }

    new_hash->hashsize = hashsize;
    new_hash_array = calloc (hashsize, sizeof(hasharray_t));
    if(!new_hash_array){
        free(new_hash);
        fprintf(stderr, "ERROR: Memory allocation for hash_array failed!\n");
        return NULL;
    }

    new_hash_array->entries = NULL;
    new_hash_array->entries = 0;
    new_hash->hasharrays = new_hash_array;
    return new_hash;
}

void hash_insert(hash_t *hash, char songtitle[], char interpreter[]){
    long index = 0;
    long *entry_index = NULL;
    entry_t* hash_entries = NULL;

    index = hash_key(songtitle, interpreter, hash->hashsize);
    if(hash->hasharrays[index].entries == 0){
        hash_entries = calloc(1, sizeof(entry_t));
        if(!hash_entries){
            fprintf(stderr, "ERROR: Memory allocation for hash_entry failed!\n");
            return;
        }
        hash->hasharrays[index].entries = hash_entries;
        entry_index = &(hash->hasharrays[index].num_entries);
    }
    else if(hash->hasharrays[index].entries > 0){
        hash_entries = realloc(hash->hasharrays[index].entries,(1 + hash->hasharrays[index].num_entries) * sizeof(entry_t));
        if(!hash_entries){
            fprintf(stderr, "ERROR: Memory reallocation for hash_entry failed!\n");
            return;
        }
        hash->hasharrays[index].entries = hash_entries;
        entry_index = &(hash->hasharrays[index].num_entries);
        hash_entries = &(hash_entries[*entry_index]);
    }
    strcpy(hash_entries->songtitle, songtitle);
    strcpy(hash_entries->interpreter, interpreter);
    *entry_index += 1;
    return;
}

//TASK F
void hash_print(hash_t *hash){
    long i = 0, j = 0;
    if(hash){
        for( i = 0; i < hash->hashsize; i++){
            if(hash->hasharrays[i].entries != NULL){
                for(j = 0; j < hash->hasharrays[i].num_entries; j++){
                    printf("%ld.%ld\t %s - %s\n", i,j, hash->hasharrays[i].entries[j].songtitle, hash->hasharrays[i].entries[j].interpreter);
                }
            }
        }
        return;
    }
    fprintf(stderr, "ERROR: Hash does not exist!\n");
    return;
}

void hash_free(hash_t *hash){
    long i = 0;
    if(hash){
        for( i = 0; i < hash->hashsize; i++){
            if(hash->hasharrays[i].entries != NULL){
                free(hash->hasharrays[i].entries);
            }
        }
        free(hash->hasharrays);
        free(hash);
        return;
    }
    fprintf(stderr, "ERROR: Hash does not exist!\n");
    return;
}

hash_t *convert_heap_to_hash(heap_t *heap){
    hash_t * new_hash = NULL;
    long i = 0;
    if(!heap){
        fprintf(stderr,"ERROR: Heap does not exist!\n");
        return NULL;
    }
    new_hash = hash_create(heap->size *3);
    if(!new_hash){
        fprintf(stderr, "ERROR: Memory allocation for hash failed!\n");
        return NULL;
    }
    for ( i = 1; i < heap->next_free; i++){
        hash_insert(new_hash, heap->entries[i].songtitle, heap->entries[i].songtitle);
    }
    return new_hash;
}

//TASK G
entry_t *hash_search(hash_t *hash, char songtitle[], char interpreter[]){
    long hash_index = hash_key(songtitle, interpreter, hash->hashsize);
    long entry_indices = hash->hasharrays[hash_index].num_entries;
    entry_t *hash_entries = hash->hasharrays[hash_index].entries;
    long j = 0;

    if (hash_entries != NULL){
        for (j = 0; j < entry_indices; j++){
            if(strcmp(hash_entries[j].songtitle, songtitle)==0){
                if(strcmp(hash_entries[j].interpreter, interpreter)==0)
                    return &(hash_entries[j]);
            }
        }
    }
    return NULL;
}

long hash_delete_entry(hash_t * hash, char songtitle[], char interpreter[]){
    long hash_index = hash_key(songtitle, interpreter, hash->hashsize);
    long *entry_indices = &(hash->hasharrays[hash_index].num_entries);
    entry_t *hash_entries = hash->hasharrays[hash_index].entries;
    long j = 0;

    if (hash_entries != NULL){
        for (j = 0; j < *entry_indices; j++){
            if(strcmp(hash_entries[j].songtitle, songtitle)==0){
                if(strcmp(hash_entries[j].interpreter, interpreter)==0){
                    if(*entry_indices == 1){
                        free(hash_entries);
                        *entry_indices = 0;
                        hash->hasharrays[hash_index].entries = NULL;
                    }
                    else{
                        if(*entry_indices != 1 + j)
                            memmove(&(hash_entries[j]), &(hash_entries[j+1]), (*entry_indices - j -1)*sizeof(entry_t));
                        *entry_indices -= 1;
                        hash_entries = realloc(hash_entries, sizeof(entry_t) * (*entry_indices));
                        hash->hasharrays[hash_index].entries = hash_entries;
                    }
                    return 1;
                }
            }
        }
    }
    fprintf(stderr, "No matching entry found!\n");
    return 0;
}

//TASK H
void benchmark() {
    hash_t *bm_hash = NULL;
    heap_t * bm_heap = NULL;
    long steps[] = {100, 200 ,400, 800, 1600, 3200, 6400, 12800, 25600, 51200};//, 51200*2 , 51200*4, 51200*8, 51200*16};
    long num_steps = sizeof(steps) / sizeof(long);
    long i = 0, j = 0;
    double fill_time, search_time,compare_calls = 0;
    char **string_array = NULL;
    clock_t timeStart = 0;
    printf("========================BENCHMARK MODE========================\n\n"
           "Hp/Hs\t|N\t| Fill time (ms) \t| Avg. search time (ms)\t| Avg. compare call\n"
           "---------------------------------------------------------------\n");

    for(j = 0; j < num_steps; j++){

        string_array = generate_random_string_array(steps[j], TXT_LEN-1);
        bm_heap = heap_create(steps[j]+1);

        timeStart = clock();
        for (i = 0; i < steps[j]; i++)
            heap_insert(bm_heap,string_array[i], string_array[i]);

        fill_time = (double) (clock() - timeStart) * 1000.0 / CLOCKS_PER_SEC;

        timeStart = clock();
        for (i = 0; i < steps[j]; i++)
            heap_search(bm_heap, string_array[i], string_array[i]);
        search_time = (double) (clock() - timeStart) * 1000.0 / CLOCKS_PER_SEC;
        search_time /= (double)steps[j];

        printf("Hp\t|%ld\t| %.3f \t\t| %.3f \t\t| %.3f\t\n", steps[j], fill_time, search_time, compare_calls);

        timeStart = clock();
        bm_hash = hash_create(steps[j]*3);
        for (i = 0; i < steps[j]; i++)
            hash_insert(bm_hash, string_array[i], string_array[i]);
            //bm_hash = convert_heap_to_hash(bm_heap);
        fill_time = (double) (clock() - timeStart) * 1000.0 / CLOCKS_PER_SEC;

        timeStart = clock();
        for (i = 0; i < steps[j]; i++)
            hash_search(bm_hash, string_array[i], string_array[i]);
        search_time = (double) (clock() - timeStart) * 1000.0 / CLOCKS_PER_SEC;
        search_time /= (double)steps[j];

        printf("Hs\t|%ld\t| %.3f \t\t| %.3f \t\t| %.3f\n", steps[j], fill_time, search_time, compare_calls);
        heap_free(bm_heap);
        bm_heap = NULL;
        hash_free(bm_hash);
        bm_hash = NULL;
        free_string_array(string_array,steps[j] );
    }
}

//BENCHMARK EXAMPLE COPIED
char *get_random_string(size_t length){
    if (!length)
        return 0;

    size_t key, i;
    static char charset[] = "abcdefghijklmnopqrstuvwxyz"
                              "ABCDEFGIJKLMNOPQRSTUVWXYZ"
                              "0123456789";
    size_t l = (int)(sizeof(charset)/sizeof(charset[0]) - 1);
    char *rnd_string = malloc((length+1)*sizeof(char));

    if (rnd_string){
        for (i=0; i<length; i++){
            key = rand() % l;
            rnd_string[i] = charset[key];
        }
        rnd_string[length] = '\0';
        return rnd_string;
    }
    else
        return 0;
}

char **generate_random_string_array(size_t array_size, size_t string_length){

    if (!array_size || !string_length)
        return 0;

    int i;
    char **rnd_string_array = malloc(array_size*sizeof(char *));

    if (!rnd_string_array)
        return 0;
    for (i=0; i<array_size; i++)
        rnd_string_array[i] = get_random_string(string_length);

    return rnd_string_array;
}

void free_string_array(char **string_array, size_t array_size){
    long i;

    if (string_array == NULL || array_size == 0)
        return;

    for (i=0; i<array_size; i++){
        if (string_array[i]){
            free(string_array[i]);
            string_array[i] = NULL;
        }
    }

    free(string_array);
    string_array = 0;
}
