/************************* File: container.h ************************/
#ifndef CONTAINER_H_INCLUDED
#define CONTAINER_H_INCLUDED

#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* create a local variable timeStart to remember the start time */
#define START_TIMING clock_t timeStart = clock();
/* calculate difference of current time and timeStart to measure the time */
#define STOP_TIMING  (double) (clock() - timeStart) * 1000 / CLOCKS_PER_SEC

typedef struct entry_s {
  char data[256];
} entry_t;

char *get_random_string(size_t length);
char **generate_random_string_array(size_t array_size, size_t string_length);
void free_string_array(char **string_array, size_t array_size);

entry_t *create_container(char **string_array, size_t array_size, size_t string_length, double *cpu_time);
entry_t *search_in_container(char *search_string, entry_t *container, size_t container_size, double *cpu_time, long *n_compare);
void measure_container(size_t container_sizes[], size_t num_sizes , size_t string_length);

#endif // CONTAINER_H_INCLUDED
