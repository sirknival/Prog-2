#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

//-----------------------LIBRARIES-------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "duma.h"
//-----------------------TYPEDEFS-------------------------
typedef struct Element_s {
    char songtitle[256], interpreter[256];
    struct Element_s *next, *prev;
} Element_t;

typedef long (*element_compare) (Element_t *, Element_t *);

//-----------------------HELPER FUNC-------------------------
long readValue(char *name);
void my_getline(char *destination, long len);
Element_t *get_start(Element_t *list);
//-----------------------FUNCTIONS-------------------------
void read_from_keyboard(Element_t *item);
Element_t *allocate_element();
Element_t *insert_last(Element_t *list);
void free_list(Element_t **list);
void print_single_element(Element_t *element);
void print_entire_list(Element_t * list);
long elements_count(Element_t *list);
Element_t *element_at(Element_t *list, long index);
Element_t *insert_at(Element_t *list, long index);
Element_t *delete_at(Element_t *list, long index);
void print_list_reverse(Element_t* list);
void print_list_random(Element_t *list);
long compare_songtitles_larger(Element_t *A, Element_t *B);
long compare_songtitles_smaller(Element_t *A, Element_t *B);
Element_t *sort_list(Element_t *list, element_compare fkt);
Element_t *unique_list(Element_t *list);
Element_t *rotate_list(Element_t *list, long shift);

#endif // FUNCTIONS_H_INCLUDED
