#include "operations.h"

// HELPER FUNCTIONS
long readValue(char *name) {
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

vector_t *choose_vec(vector_t *vec_1,vector_t *vec_2, vector_t *vec_3, vec_array_t* array){
    long option = 0;
    printf("You can choose between \n\tVector A (1) \n\tVector B (2) \n\tVector C (3) \n\tVector from the array (0)\n");
    while (1){
        option = readValue ("Select");
        switch(option){
            case 0:
                printf("The array has %ld vectors inside. Choose by stating an index between 0 and %ld.\n", array->length, array->length-1);
                option = readValue("Index");
                if (option >= 0 && option < array->length)
                    return array->vectors[option];
                else
                    printf("You stated an invalid index");
                break;
            case 1: return vec_1;
            case 2: return vec_2;
            case 3: return vec_3;
            default: printf("Option does not exist!\n");
        }
    }
    return NULL;
}

// TASK A
error_t vector_allocate(vector_t *vec, long dim) {
    long *new_vector = NULL;
    //Allocate memory to null-ptr only
    if(vec->values == NULL) {
        //Check if dim is valid
        if (dim < 0)
            return DIMENSION_MISMATCH_ERROR;
        //Allocate memory
        if (( new_vector = calloc(dim, sizeof(long))) != NULL) {
            vec->values = new_vector;
            vec->dim = dim;
            return SUCCESS;
        }
    }
    return ALLOC_FAILURE_ERROR;
}

error_t vector_clear(vector_t *vec) {
    //Check if ptr is valid
    if(vec->values != NULL){
        //Free ptr and set ptr to NULL
        free(vec->values);
        vec->values = NULL;
        vec->dim = 0;
        return SUCCESS;
    }
    return NULL_POINTER_ERROR;
}

error_t vector_fill(vector_t *vec, long index, long value) {
    //Check if ptr is valid
    if (vec->values != NULL){
        //Check if dim is valid
        if(index < vec->dim && index >= 0) {
            //Set value
            vec->values[index] = value;
            return SUCCESS;
        }
        else
            return OUT_OF_RANGE_ERROR;
    }
    return NULL_POINTER_ERROR;
}

error_t vector_print(vector_t *vec) {
    long i = 0;
    //Check if ptr is valid
    if(vec->values != NULL){
        //Print all values
        printf("Elements: [ ");
        for (; i < vec->dim; i++)
            printf("%ld ", vec->values[i]);
        printf("]\n");
        return SUCCESS;
    }
    return NULL_POINTER_ERROR;
}

//TASK B
error_t vector_change_dim(vector_t *vec, long new_dim) {
    long *new_vector = NULL;
    //Check if new dimension is valid (higher than zero)
    if(new_dim < 1)
        return OUT_OF_RANGE_ERROR;
    //Allocate memory
    if((new_vector= realloc(vec->values, sizeof(long)*new_dim)) != NULL) {
        vec->values = new_vector;
        if(vec->dim < new_dim){
            //Fill all values with zero and set dim
            while(vec->dim < new_dim) {
                vec->values[vec->dim] = 0;
                vec->dim += 1;
            }
        }
        else if (vec->dim > new_dim)
            vec->dim = new_dim;
        return SUCCESS;
    }
    return ALLOC_FAILURE_ERROR;
}

error_t vector_add(vector_t *vec_a, vector_t *vec_b, vector_t *vec_sum) {
    long i = 0;
    //Check if vec a and b are valid
    if (vec_a->values != NULL && vec_a->values != NULL){
        //Check if dim a equals dim b
        if (vec_a->dim == vec_b->dim){
            //If sum vec is allocated, clear sum vec
            if(vec_sum->values != NULL)
                vector_clear(vec_sum);
            //Create a vec with matching dim
            vector_allocate(vec_sum, vec_a->dim);
            //Calc sum elementwise and store results in sum vec
            for (; i < vec_a->dim; i++)
                vec_sum->values[i]= vec_a->values[i] + vec_b->values[i];
            return SUCCESS;
        }
        return DIMENSION_MISMATCH_ERROR;
    }
    return NULL_POINTER_ERROR;
}

error_t vector_slice(vector_t *vec, long index_a, long index_b) {
    long *temp_vec = NULL;
    long slice_size = 0;
    //Check it vec is valid
    if (vec->values != NULL){
        //Check if indices are valid
        if (0 <= index_a && index_a <= index_b && index_b < vec->dim){
            slice_size = index_b-index_a;
            //Allocate memory to a vec with fitting size
            if ( (temp_vec = calloc(slice_size, sizeof(long))) != NULL) {
                //Copy vec to fitting vec (new allocated memory)
                memcpy(temp_vec, &(vec->values[index_a]), slice_size*sizeof(long));
                //Clear old vec (memory)
                vector_clear(vec);
                //Copy adress of memory to new vec and set correct dim
                vec->values = temp_vec;
                vec->dim = slice_size;
                return SUCCESS;
            }
            return ALLOC_FAILURE_ERROR;
        }
        return OUT_OF_RANGE_ERROR;
    }
    return NULL_POINTER_ERROR;
}

//TASK C
vector_t *vector_allocate_dynamic(long dim) {
    vector_t *vec_new = NULL;
    //Allocate memory for new vector ptr
    if(( vec_new = malloc(sizeof(vector_t))) != NULL){
        //Init new vector
        vec_new->dim = 0;
        vec_new->values = NULL;
        //Allocate memory and set correct dim
        vector_allocate(vec_new, dim);
        return vec_new;
    }
    return NULL;
}

error_t vector_free(vector_t *vec) {
    error_t report = SUCCESS;
    //Check it vec is valid
    if(vec->values != NULL){
        //Clear memory used for vector values
        report = vector_clear(vec);
        if (report != SUCCESS)
            return report;
        //Clear vector ptr memory and set to NULL
        free(vec);
        vec = NULL;
        return SUCCESS;
    }
    return NULL_POINTER_ERROR;
}

//TASK D
vec_array_t* vec_array_allocate(long length) {
    long i = 0;
    vec_array_t *data_array =  NULL;
    //Allocate memory for vector array
    if( (data_array = malloc(sizeof(vec_array_t))) != NULL){
        //Allocate memory for vector pointer pointer
        if((data_array->vectors = calloc(length, sizeof(vector_t*) ))!= 0){
            for(; i < length; i++)
                //Fill array with vectors of length 1
                data_array->vectors[i] = vector_allocate_dynamic(1);
            data_array->length = length;
            return data_array;
        }
    }
    return NULL;
}

error_t vec_array_store(vec_array_t* array, vector_t* vec) {
    vector_t **temp_array = NULL;
    long old_length = array->length;
    long i = 0;
    //Reallocate memory - increase by one mem space
    if((temp_array = realloc(array->vectors, (old_length+1)*sizeof(vector_t*)))!=NULL){
        array->vectors = temp_array;
        array->length = old_length + 1;
        //Allocate memory for additional vector
        if( (array->vectors[old_length] = calloc(1, sizeof(vector_t))) == NULL)
            return ALLOC_FAILURE_ERROR;
        //Allocate memory for long values
        if( (array->vectors[old_length]->values = calloc(vec->dim, sizeof(long))) == NULL)
            return ALLOC_FAILURE_ERROR;
        //Copy all vector values into vector array elementwise
        for(i = 0; i < vec->dim; i++)
            array->vectors[old_length]->values[i]=vec->values[i];
        array->vectors[old_length]->dim = vec->dim;
        return SUCCESS;
    }
    return ALLOC_FAILURE_ERROR;
}

error_t vec_array_free(vec_array_t* array) {
    error_t response = SUCCESS;
    long j = 0;
    //Check if ptr is valid
    if(array != NULL){
        //FRee all elements (long) from vector
        for (j = 0; j < array->length; j++)
            response = vector_free(array->vectors[j]);
        //Free vector pointer array
        free(array->vectors);
        //Free vector array
        free(array);
        return response;
    }
    return NULL_POINTER_ERROR;
}

error_t vec_array_delete_at(vec_array_t *array, long index){
    error_t response = SUCCESS;
    vector_t **temp_array = NULL;
    long old_length = array->length;
    if (array != NULL){
        if(index < old_length && index >= 0) {
            response = vector_free(array->vectors[index]);
            memmove(&(array->vectors[index]),&(array->vectors[index+1]), (old_length - index-1) * sizeof(vector_t*));
            if( (temp_array = realloc(array->vectors, sizeof(vector_t*) * (old_length -1))) != NULL){
                array->vectors = temp_array;
                array->length -= 1;
                return response;
            }
            else
                return ALLOC_FAILURE_ERROR;

        }
        else
            return OUT_OF_RANGE_ERROR;
    }
    return NULL_POINTER_ERROR;
}

//TASK E
error_t vec_array_save(vec_array_t * array, char *file_name) {
    FILE *file;
    long i = 0, j = 0;
    if((file = fopen(file_name, "w"))){
        for (i = 0;i < array->length; i++){
            fprintf(file, "[ ");
            for (j = 0; j < array->vectors[i]->dim; j++){
                fprintf(file, "%ld ", array->vectors[i]->values[j]);
            }
            fprintf(file, "]\n");
        }
        fclose(file);
        return SUCCESS;
    }
    return FILE_ACCESS_ERROR;
}

vec_array_t* vec_array_load(char *file_name) {
    FILE *file;
    int parsed_num = 0;
    long number_count = 0;
    long line_begin = 0;
    long cur_buf_index = 0;
    long vec_index = 0;
    char buffer_number[15] = "";
    char read_char = ' ';
    vector_t* buffer_vec = NULL;
    vec_array_t *new_array = vec_array_allocate(1);

    if((file = fopen(file_name, "r"))){
        while( fscanf(file, "%c", &read_char) != EOF){
            if (read_char == '[')
                line_begin = ftell(file);
            if (isdigit(read_char))
                parsed_num = 1;
            if (read_char == ' ' && parsed_num == 1){
                number_count++;
                parsed_num = 0;
            }
            if (read_char == ']') {
                buffer_vec = vector_allocate_dynamic(number_count);
                fseek(file, line_begin, 0);
                vec_index = 0;
                 while( fscanf(file, "%c", &read_char) != EOF){
                    if (isdigit(read_char)){
                        buffer_number[cur_buf_index++] = read_char;
                    }
                    if (read_char == ' ' && cur_buf_index != 0){
                        buffer_vec->values[vec_index++] = atol(buffer_number);
                        memset(&buffer_number[0], 0, sizeof(buffer_number));
                        cur_buf_index = 0;
                    }
                    if (read_char == ']')
                        break;
                }
                vec_array_store(new_array, buffer_vec);
                vector_free(buffer_vec);
                number_count = 0;
            }
        }
        vec_array_delete_at(new_array, 0);
        return new_array;
    }
    return NULL;
}

//TASK F
double vector_norm (vector_t *vec){
    double norm = 0.0;
    long i = 0;
    for (; i < vec->dim; i++){
        norm += pow(vec->values[i], 2);
    }
    norm = sqrt(norm);
    return norm;
}

error_t vec_array_sort(vec_array_t * array, measure_f sort_key){
    long start = 0, end = 0;
    vector_t *ptr_temp = NULL;
    if (array != NULL){
        for (end = array->length-1; end > 0; end--) {
            for (start = 0; start < end; start++){
                if(sort_key(array->vectors[start]) > sort_key(array->vectors[start+1])){
                    ptr_temp = array->vectors[start];
                    array->vectors[start] = array->vectors[start+1];
                    array->vectors[start+1] = ptr_temp;
                }
            }
        }
        return SUCCESS;
    }
    return NULL_POINTER_ERROR;
}


