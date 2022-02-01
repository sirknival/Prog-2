/************************* File: main.c ************************/
#include <stdio.h>
#include <stdlib.h>
#include "container.h"

int main()
{
  //srand(time(27)); // fixed seed, the same pseudo-random strings will be generated every time
  srand(time(0));    // varying seed, generate different pseudo-random strings every time
  size_t container_sizes[] = { 100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200 };
  measure_container(container_sizes, 10, 10);
  return 0;
}
