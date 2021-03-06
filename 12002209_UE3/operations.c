#include "operations.h"

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

node_t * tree_top(node_t *root_node){
    while(root_node->parent != NULL)
        root_node = root_node->parent;
    return root_node;
}

node_t *find_smallest_node(node_t *rootnode){
    while(rootnode->left != NULL){
        rootnode = rootnode->left;
    }
	return rootnode;
}

void print_inorder(node_t *rootnode){
    if (rootnode == NULL)
        return;
    print_inorder(rootnode->left);
    printf(" %s - %s\n", rootnode->songtitle, rootnode->interpreter);
    print_inorder(rootnode->right);
}


//TASK A
node_t *create_node(char songtitle[], char interpreter[]) {
    node_t* new_node = NULL;
    new_node = malloc(sizeof(node_t));
    if(new_node){
        new_node->parent= NULL;
        new_node->right = NULL;
        new_node->left = NULL;
        strcpy(new_node->interpreter, interpreter);
        strcpy(new_node->songtitle, songtitle);
    }
    return new_node;
}

node_t *insert_node(node_t *rootnode, char songtitle[], char interpreter[]) {
    node_t *temp_node = NULL;
    temp_node = create_node(songtitle, interpreter);

    if (rootnode == NULL)
        return temp_node;
    do {
        if(strcmp(rootnode->songtitle,temp_node->songtitle)>=0){
            //printf("Left - ");
            if(rootnode->left == NULL){
                //printf("Placed\n");
                rootnode->left = temp_node;
                temp_node->parent = rootnode;
                break;
            }
            rootnode = rootnode->left;
        }
        else {
            //printf("Right - ");
            if(rootnode->right == NULL){
                //printf("Placed\n");
                rootnode->right = temp_node;
                temp_node->parent = rootnode;
                break;
            }
            rootnode = rootnode->right;
        }
    }while(rootnode != NULL);
    return tree_top(rootnode);
}

//TASK B
void print_tree (node_t* rootnode){
    static long level = 0;
    long i = 0;
    if (rootnode == NULL){
        level--;
        return;
    }
    for(i = 0; i < level; i++)
        printf("    ");
    if (level != 0)
        printf("%c%c%c", 192, 196, 196);
    printf(" %s - %s\n", rootnode->songtitle, rootnode->interpreter);
    level++;
    print_tree(rootnode->left);
    level++;
    print_tree(rootnode->right);
    level--;
    if(rootnode->parent == NULL)
        level = 0;
}

void destroy_tree(node_t *rootnode) {
    if(rootnode == NULL)
        return;
    destroy_tree(rootnode->left);
    destroy_tree(rootnode->right);

    free(rootnode);
    return;
}

//TASK C
long count_nodes(node_t *rootnode) {
    long counter = 1;
    if (rootnode == NULL)
        return 0;
    counter += count_nodes(rootnode->left);
    counter += count_nodes(rootnode->right);

    return counter;
}

node_t *search_node(node_t *rootnode, char songtitle[]){
    node_t *result = NULL;
    if (rootnode != NULL){
        if(strcmp(rootnode->songtitle,songtitle)>0)
            result = search_node(rootnode->left, songtitle);
        else if(strcmp(rootnode->songtitle,songtitle)<0)
            result = search_node(rootnode->right, songtitle);
        else
            return rootnode;
    }
    return result;
}

//TASK D
node_t **search_all(node_t *rootnode, char interpreter[], long *found){
    static node_t **result_array = NULL;
    if(*found == 0) result_array = NULL;

    if(rootnode == NULL)
        return result_array;

    result_array = search_all(rootnode->left, interpreter, found);

    if(strcmp(rootnode->interpreter, interpreter)==0){
        (*found)++;
        if(result_array == NULL)
            result_array = calloc(1, sizeof(node_t *));
        else
            result_array = realloc(result_array, sizeof(node_t*)*(*found));
        *(result_array + (*found)-1) = rootnode;
    }

    result_array = search_all(rootnode->right, interpreter, found);

    return result_array;
}

//TASK E
long max_depth(node_t *rootnode) {
    long depth_left = 0, depth_right = 0;

    if (rootnode == NULL)
        return 0;

    depth_left = max_depth(rootnode->left);
    depth_right = max_depth(rootnode->right);

    return (depth_left > depth_right ? ++depth_left : ++depth_right);
}

void print_level(node_t *rootnode, long level){
    if (rootnode == NULL)
        return;
    if (level == 0){
        printf("%s - %s\n", rootnode->songtitle, rootnode->interpreter);
        return;
    }

    print_level(rootnode->left, level-1);
    print_level(rootnode->right, level -1);
}

//TASK F
node_t *delete_node(node_t *rootnode, char songtitle[], char interpreter[]){
    node_t *temp_ptr = NULL;
    if (rootnode == NULL)
        return NULL;
    else if(strcmp(rootnode->songtitle, songtitle)>0) //Smaller song title
        rootnode->left = delete_node(rootnode->left, songtitle, interpreter);
    else if(strcmp(rootnode->songtitle, songtitle)<0) //Bigger song title
        rootnode->right = delete_node(rootnode->right, songtitle, interpreter);
    else if(strcmp(rootnode->interpreter, interpreter)!=0) //Song title match, but not interpreter
        rootnode->left = delete_node(rootnode->left, songtitle, interpreter);
    else { //matched
        if(rootnode->left == NULL && rootnode->right == NULL){
            rootnode->parent = NULL;
            free(rootnode);
            rootnode = NULL;
        }
        else if (rootnode->left == NULL){
            temp_ptr = rootnode;
            rootnode = rootnode->right;
            rootnode->parent = temp_ptr->parent;
            rootnode->parent->right = rootnode;
            free(temp_ptr);
        }
        else if (rootnode->right == NULL){
            temp_ptr = rootnode;
            rootnode = rootnode->left;
            rootnode->parent = temp_ptr->parent;
            rootnode->parent->left = rootnode;
            free(temp_ptr);
        }
        else {
            temp_ptr = find_smallest_node(rootnode->right);
            strcpy(rootnode->songtitle,temp_ptr->songtitle);
            strcpy(rootnode->interpreter, temp_ptr->interpreter);
			rootnode->right = delete_node(rootnode->right,rootnode->songtitle, rootnode->interpreter);
        }
    }
    return rootnode;
}

//TASK G
double average_comparisons(node_t* rootnode){
    double sum = 0;
    static double level = 0;

    if (rootnode == NULL)
        return 0;
    if (rootnode->parent == NULL)
        level = 0;

    level++;
    sum += level;
    sum += average_comparisons(rootnode->left);
    sum += average_comparisons(rootnode->right);
    level--;

    if (rootnode->parent == NULL){
        level = 0;
        return sum / count_nodes(rootnode);
    }

    return sum;
}

//Copied from benchmark-example
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

//Copied from benchmark-example
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
//Copied from benchmark-example
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

void benchmark(){
    FILE *file = NULL;
    node_t *tree_benchmark = NULL;
    long steps[] = {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 51200*2 , 51200*4, 51200*8, 51200*16, 51200*32, 51200*64};
    long num_steps = sizeof(steps) / sizeof(long);
    long i = 0, j = 0, string_len = 10;
    double fill_time[num_steps],search_time[num_steps],compare_calls[num_steps] ;
    char **string_array = NULL;

    clock_t timeStart = 0;

    for (j = 0; j < num_steps; j++){
        string_array = generate_random_string_array(steps[j], string_len);

        timeStart = clock();
        for (i = 0; i < steps[j]; i++)
            tree_benchmark = insert_node(tree_benchmark, string_array[i], string_array[i]);
        fill_time[j] = (double) (clock() - timeStart) * 1000.0 / CLOCKS_PER_SEC;
        compare_calls[j] = average_comparisons(tree_benchmark);

        timeStart = clock();
        for (i = 0; i < steps[j]; i++)
            search_node(tree_benchmark, string_array[i]);

        search_time[j] = (double) (clock() - timeStart) * 1000.0 / CLOCKS_PER_SEC;
        search_time[j] /= (double)steps[j];

        destroy_tree(tree_benchmark);
        tree_benchmark = NULL;
        free_string_array(string_array, steps[j]);
    }

    printf("========================BENCHMARK MODE========================\n\n"
           "N\t| Fill time (ms) \t| Avg. search time (ms)\t| Avg. compare call\n"
           "---------------------------------------------------------------\n");
    for (j = 0; j < num_steps; j++)
        printf("%ld\t| %.3f \t\t| %.3f \t\t| %.3f\n", steps[j], fill_time[j], search_time[j], compare_calls[j]);

    file = fopen("benchmark.txt", "w");
    setlocale(LC_NUMERIC, "French_Canada.1252");

    if (file){
        for (j = 0; j < num_steps; j++)
            fprintf(file, "%ld\t| %.3f \t\t| %.3f \t\t| %.3f\n", steps[j], fill_time[j], search_time[j], compare_calls[j]);
        fclose(file);
    }

}
//TEIL H
long check_if_order_OK(node_t *rootnode){
    long chk_flag = 0;
    if(rootnode == NULL)
        return 0;
    if(rootnode->left != NULL){
        if(strcmp(rootnode->songtitle, rootnode->left->songtitle)<0)
           chk_flag = 1;
        check_if_order_OK(rootnode->left);
    }
    if(rootnode->right!= NULL){
        if(strcmp(rootnode->songtitle, rootnode->right->songtitle)>=0)
           chk_flag = 1;
        check_if_order_OK(rootnode->right);
    }
    return chk_flag;
}

void tree_data_to_array(node_t* rootnode, char**songs, char**singer, long *cur_index){
    if(rootnode == NULL)
        return;
    tree_data_to_array(rootnode->left, songs, singer, cur_index);
    strcpy(songs[*cur_index], rootnode->songtitle);
    strcpy(singer[*cur_index], rootnode->songtitle);
    (*cur_index)++;
    tree_data_to_array(rootnode->right, songs, singer, cur_index);
    return;
}

node_t* plant_balanced_tree(node_t* parentnode, char** songtitles, char **interpreter, long begin, long end){
    long middle = (begin + end)/2;

    node_t* rootnode = insert_node(parentnode, songtitles[middle], interpreter[middle]);
    if((middle - begin)> 1)
        plant_balanced_tree(rootnode, songtitles, interpreter,begin, middle);
    if((end - middle)> 1)
        plant_balanced_tree(rootnode, songtitles, interpreter,middle, end);
    return rootnode;
}

node_t *rebalance_tree(node_t *rootnode){
    if (rootnode == NULL)
        return NULL;
    long nodes = count_nodes(rootnode);
    long cur_nodes = 0;
    long i = 0;
    char **songtitles = NULL;
    char **interpreters = NULL;
    songtitles = calloc(nodes, sizeof(char*));
    interpreters = calloc(nodes, sizeof(char*));
    for (i = 0; i < nodes; i++){
        songtitles[i] = calloc(TXTLEN, sizeof(char));
        interpreters[i] = calloc(TXTLEN, sizeof(char));
    }

    tree_data_to_array(rootnode, songtitles, interpreters, &cur_nodes);
    destroy_tree(rootnode);
    rootnode = plant_balanced_tree(NULL, songtitles, interpreters,0, nodes);
    rootnode = insert_node(rootnode, songtitles[0], interpreters[0]);
    rootnode->parent = NULL;

    for (i = 0; i < nodes; i++){
       free(songtitles[i]);
       free(interpreters[i]);
    }
    free(interpreters);
    free(songtitles);
    return NULL;
}

