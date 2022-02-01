/************************* File: container.c ************************/
#include "container.h"

/* create a random alphanumeric string */
char *get_random_string(size_t length)
{
  if (!length) { return 0; }

  size_t key, i;
  static char charset[] = "abcdefghijklmnopqrstuvwxyz"
                          "ABCDEFGIJKLMNOPQRSTUVWXYZ"
                          "0123456789";

  size_t l = (int)(sizeof(charset)/sizeof(charset[0]) - 1);

  char *rnd_string = malloc((length+1)*sizeof(char));

  if (rnd_string)
  {
    for (i=0; i<length; i++)
    {
      key = rand() % l;
      rnd_string[i] = charset[key];
    }
    rnd_string[length] = '\0';
    return rnd_string;
  }
  else
    return 0;
}

/* creates an array of strings with length string_length */
char **generate_random_string_array(size_t array_size, size_t string_length)
{
  if (!array_size || !string_length) { return 0; }

  int i;
  char **rnd_string_array = malloc(array_size*sizeof(char *));

  if (!rnd_string_array) { return 0; }

  for (i=0; i<array_size; i++)
    rnd_string_array[i] = get_random_string(string_length);

  return rnd_string_array;
}

void free_string_array(char **string_array, size_t array_size)
{
  if (!string_array || !array_size) { return; }

  int i;

  for (i=0; i<array_size; i++)
    if (string_array[i])
    {
        free(string_array[i]);
        string_array[i] = 0;
    }

  free(string_array);
  string_array = 0;
}

/* create a container from string_array */
entry_t *create_container(char **string_array, size_t array_size, size_t string_length, double *cpu_time)
{
  if (!string_array || !array_size || !string_length) { return 0; }

  int i;
  entry_t *new_container = calloc(array_size, sizeof(entry_t));

  if (!new_container) { return 0; }

  START_TIMING; /* start CPU timer */
  for (i=0; i<array_size; i++)
    strncpy(new_container[i].data, string_array[i], string_length);
  *cpu_time = STOP_TIMING; /* stop timer and convert to milliseconds */

  return new_container;
}

/* search for an entry in the container */
entry_t *search_in_container(char *search_string, entry_t *container, size_t container_size, double *cpu_time, long *n_compare)
{
  if (!search_string || !container || !container_size) { return 0; }

  int i;
  *n_compare = 0;

  START_TIMING;
  for (i=0; i<container_size; i++)
  {
    (*n_compare)++; /* increment the compare counter */
    if (!strcmp(container[i].data, search_string))
    {
      *cpu_time = STOP_TIMING;
      return &container[i];
    }
  }
  *cpu_time = STOP_TIMING;
  return 0;
}

/* perform run time analysis for different container sizes */
void measure_container(size_t container_sizes[], size_t num_sizes , size_t string_length)
{
  if (!container_sizes || !num_sizes || !string_length) { return; }

  int i, j;
  double t_fill, t_search, t_search_mean;
  long n_compare, n_compare_mean;

  printf("N\t| Fill Time(ms)\t| Mean Search Time(ms)\t| Mean Compare Calls\n");
  printf("================================================================\n");

  for (i=0; i<num_sizes; i++)
  {
    t_search_mean = 0;
    n_compare_mean = 0;

    char **string_array = generate_random_string_array(container_sizes[i], string_length);
    entry_t *container = create_container(string_array, container_sizes[i], string_length, &t_fill);

    for (j=0; j<container_sizes[i]; j++)
    {
      search_in_container(string_array[j], container, container_sizes[i], &t_search, &n_compare);
      t_search_mean += t_search;
      n_compare_mean += n_compare;
    }
    t_search_mean = t_search_mean / container_sizes[i];
    n_compare_mean = n_compare_mean / container_sizes[i];

    printf("%7ld\t| %1.6lf\t| %1.6lf\t\t| %ld\n", container_sizes[i], t_fill, t_search_mean, n_compare_mean);

    free_string_array(string_array, container_sizes[i]);
    free(container);
  }
}
