#include "functions.h"

// HELPER FUNCTIONS
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

void my_getline(char *destination, long len){
    /*Reads from stream stdin a string with certain length and stores it under specifies adress*/
    char input[len];
    long index = 0;
    char cur_char = ' ';

    memset(&input[0], 0, sizeof(input));
    for(index = 0; index+1 < len; index++){
        cur_char = getchar();
        if(cur_char == '\n')
            break;
        input[index] = cur_char;
    }
    input[index] = '\0';
    fflush(stdin);
    memcpy(destination, &input, len);
    return;
}

void read_from_keyboard(Element_t *item) {
    /*Reads db entry (song, singer) from the terminal ans stores values*/
    if (item != NULL){
        printf("Please enter some information\n");
        printf("Songtitle:\t");
        my_getline(item->songtitle, 256);
        printf("Interpret:\t");
        my_getline(item->interpreter, 256);
    }
    else
        fprintf(stderr, "ERROR: Pointer item is invalid!\n");
}

Element_t *get_start(Element_t *list){
    /*Returns the address of the first element of a list*/
    while (list->prev != NULL)
        list = list->prev;
    return list;
}

Element_t *allocate_element(){
    /*Allocates memory of a db entiry*/
    Element_t *ptr_elem = NULL;
    if((ptr_elem = malloc(sizeof(Element_t)))!= NULL){
        ptr_elem->next = NULL;
        ptr_elem->prev = NULL;
        read_from_keyboard(ptr_elem);
        return ptr_elem;
    }
    else {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        return NULL;
    }
}

Element_t *insert_last(Element_t *list) {
    /*Appends an entiry at the end of the db*/
    Element_t *start_list = list;
    if(list != NULL ){
        Element_t *new_elem = allocate_element();
        while(list->next != NULL)
            list = list->next;
        new_elem->prev = list;
        list->next = new_elem;
        return start_list;
    }
        Element_t *new_list  = allocate_element();
        return new_list;
}

void free_list(Element_t **list) {
    /*Frees the memory of an db entry*/
    Element_t *next_elem;
    if(list == NULL){
        fprintf(stderr, "ERROR: List already freed\n");
    }
    while(*list != NULL){
        next_elem = (*list)->next;
        free(*list);
        *list = next_elem;
    }
    list = NULL;
    return;
}

void print_single_element(Element_t *element) {
    /*Prints a db entry*/
    if(element != NULL)
        printf("\t%s - %s\n", element->songtitle ,element->interpreter);
    return;
}

void print_entire_list(Element_t * list) {
    /*Prints all entries of the db*/
    long counter = 0;
    printf("Print entire list:\n");
    while(list != NULL){
        printf("%ld. ", counter++);
        print_single_element(list);
        list = list->next;
    }
    return;
}

long elements_count(Element_t *list) {
    /*Counts all entires of the db*/
    long counter = 0;
    if (list == NULL)
        return 0;
    while(list != NULL){
        counter++;
        list = list->next;
    }
    return counter;
}

Element_t *element_at(Element_t *list, long index) {
    /*Return the entry from the specifies index*/
    long counter = 0;
    if (list == NULL){
        fprintf(stderr, "ERROR: Invalid list\n");
        return NULL;
    }
    if (index < 0 || index >= elements_count(list)){
        fprintf(stderr, "ERROR: Index out of range!\n");
        return NULL;
    }
    while (counter < index) {
        list = list->next;
        counter++;
    }
    return list;
}

Element_t *insert_at(Element_t *list, long index){
    /*Inserts an element at the specifies index*/
    Element_t *new_element = NULL;
    long i = 0;

    if (list == NULL)
        return insert_last(NULL);
    if (index < 0 || index > elements_count(list)){
        fprintf(stderr, "ERROR: Index out of range!\n");
        return NULL;
    }
    if (index == elements_count(list))
        return insert_last(list);

    if ((new_element = allocate_element(NULL))== NULL){
        fprintf(stderr, "ERROR: Allocating memory failed!");
        return NULL;
    }

    for (; i < index; i ++)
        list = list->next;

    new_element->next = list;
    if(index != 0) new_element->prev = list->prev;
    if(list->prev != NULL) list->prev->next = new_element;
    list->prev = new_element;

    list = get_start(list);
    return list;
}

Element_t *delete_at(Element_t *list, long index) {
    /*Deletes an element from the specifies index*/
    long i = 0;
    long elems = elements_count(list);

    Element_t *start_element = list;

    if (list == NULL){
        fprintf(stderr,"ERROR: Invalid list\n");
        return NULL;
    }
    if (index < 0 || index >= elems){
        fprintf(stderr, "ERROR: Index out of range!\n");
        return list;
    }

    if (elems == 1){
        free_list(&list);
        return NULL;
    }

    for (; i < index; i ++)   list = list->next;

        if (index == 0){
            list->next->prev = NULL;
            start_element = list->next;
        }

        else if (index == --elems)
            list->prev->next = NULL;
        else {
            list->prev->next = list->next;
            list->next->prev = list->prev;
        }

    list->next = NULL;
    free_list(&list);

    return start_element;
}

void print_list_reverse(Element_t* list) {
    /*Print the db in reverse order*/
    long counter = elements_count(list) - 1;
    printf("Print entire list:\n");
    while(list->next != NULL)
        list = list->next;
    while(counter >= 0){
        printf("%ld. ", counter--);
        print_single_element(list);
        list = list->prev;
    }
    return;
}

void print_list_random(Element_t *list) {
    /*Prints the db in truely random order*/
    srand(time(NULL));
    long counter = elements_count(list);
    long order[counter];
    long j = 0, rand_num = 0;
    int flag_done = 0;

    for (j = 0; j < counter; j++)
        order[j] = 1;
    while(!flag_done){
        do{
            rand_num = rand()%(counter);
        }while(order[rand_num] == 0);
        order[rand_num] = 0;

        for (j = 0; j < rand_num; j++)
            list = list->next;
        printf("%ld. ", rand_num);
        print_single_element(list);
        while(list->prev != NULL)
            list = list->prev;
        rand_num = 0;
        for(j = 0; j < counter; j++)
            rand_num += order[j];
        if(rand_num == 0)
            flag_done = 1;
    }
    return;
}

long compare_songtitles_larger(Element_t *A, Element_t *B) {
    /*Compares song titles an return the larger one*/
    long state = strcmp(A->songtitle, B->songtitle);
    return (state > 0 ? 1: 0);
}

long compare_songtitles_smaller(Element_t *A, Element_t *B) {
        /*Compares song titles an return the smaller one*/
    long state = strcmp(B->songtitle, A->songtitle);
    return (state > 0 ? 1: 0);
}

Element_t *sort_list(Element_t *list, element_compare fkt) {
    /*Sorts all entries by specifies criteria - Bubble Sort*/
    /*Swapping logic based on following source: https://stackoverflow.com/questions/20095529/swapping-nodes-in-double-linked-list/27041055#27041055*/
    long start = 0, end = 0;
    Element_t *list_nxt = NULL;
    Element_t *elem_swap[4] = {NULL, NULL, NULL, NULL};

    if (list == NULL){
        fprintf(stderr, "ERROR: Invalid list\n");
        return NULL;
    }
    for (end = elements_count(list)-1; end > 0; end--) {
            for (start = 0; start < end; start++){
            list_nxt = list->next;
                if(fkt(list, list_nxt)){

                    elem_swap[0] = list->prev;
                    elem_swap[1] = list_nxt->prev;
                    elem_swap[2] = list->next;
                    elem_swap[3] = list_nxt->next;

                    list->prev = elem_swap[2];
                    list_nxt->prev = elem_swap[0];
                    list->next = elem_swap[3];
                    list_nxt->next = elem_swap[1];

                    if (list->prev != NULL) list->prev->next = list;
                    if (list->next != NULL) list->next->prev = list;

                    if (list_nxt->prev != NULL) list_nxt->prev->next = list_nxt;
                    if (list_nxt->next != NULL) list_nxt->next->prev = list_nxt;
                }
                else
                    list = list_nxt;
            }
            list = get_start(list);
        }
    list = get_start(list);
    return list;
}

Element_t *unique_list(Element_t *list) {
    /*Removes all duplicates from db*/
    long inner = 0, outer = 0, j = 0;
    long cycles = elements_count(list)-1;
    int deleteFlag[elements_count(list)];
    Element_t *start_list = list;


    for (j = 0; j < elements_count(list); j++)
        deleteFlag[j] = 0;

    for(outer = 0; outer < cycles; outer++){
        for(inner = outer; inner < cycles; inner++){
            if(strcmp(start_list->songtitle, list->next->songtitle)== 0){
                if(strcmp(start_list->interpreter, list->next->interpreter)== 0)
                deleteFlag[inner+1]=1;
            }
            list= list->next;
        }
        start_list =  start_list->next;
        list = start_list;
    }
    list = get_start(list);
    for (j = cycles; j >= 0; j--){
        if(deleteFlag[j]==1)
            list = delete_at(list, j);
    }
    return get_start(list);
}

Element_t *rotate_list(Element_t *list, long shift) {
    /*Shifts list by mod shift_num - cyclic*/
    Element_t *swap_elem = NULL;
    long i = elements_count(list);
    if (list == NULL){
        fprintf(stderr, "ERROR: invalid list\n");
        return NULL;
    }

    shift = shift % elements_count(list) +1;
    if (shift == 0)
        return list;

    swap_elem = list;
    while(list->next != NULL)
        list = list->next;
    list->next = swap_elem;
    swap_elem->prev = list;

    while(i > shift){
        list = list->prev;
        i--;
    }
    list->prev->next = NULL;
    list->prev = NULL;

    return list;
}

