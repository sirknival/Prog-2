#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>
//#include <duma.h>

#define TXTLEN 256

//Typedefs
typedef struct node_s {
    char songtitle[256], interpreter[256];
    struct node_s *parent, *left, *right;
} node_t;

//Helper
long readValue(char *name);
void my_getline(char *destination, long len, char *name);
node_t * tree_top(node_t *root_node);
void print_inorder(node_t *rootnode);

//Functions
node_t *create_node(char songtitle[], char interpreter[]);
node_t *insert_node(node_t *rootnode, char songtitle[], char interpreter[]);
void print_tree(node_t *rootnode);
void destroy_tree(node_t *rootnode);
long count_nodes(node_t *rootnode);
node_t *search_node(node_t *rootnode, char songtitle[]);
node_t **search_all(node_t *rootnode, char interpreter[], long *found);
long max_depth(node_t *rootnode);
void print_level(node_t *rootnode, long level);

node_t *delete_node(node_t *rootnode, char songtitle[], char interpreter[]);
double average_comparisons(node_t* rootnode);
void benchmark();
char *get_random_string(size_t length);
char **generate_random_string_array(size_t array_size, size_t string_length);
void free_string_array(char **string_array, size_t array_size);
long check_if_order_OK(node_t *rootnode);
node_t *rebalance_tree(node_t *rootnode);
void tree_data_to_array(node_t* rootnode, char**songs, char**singer, long *cur_index);
node_t* plant_balanced_tree(node_t* parentnode, char** songtitles, char **interpreter, long begin, long end);
#endif // OPERATIONS_H_INCLUDED

