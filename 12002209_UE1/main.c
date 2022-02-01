#include "operations.h"


//-------------------MAIN CODE-------------------
//-----------------------------------------------

int main () {
    error_t response = SUCCESS;
    int flag_terminate = 0;
    long menu_ctrl = 0;
    long dim = 2;

    char filename[30];

    vector_t *myPtr = vector_allocate_dynamic(dim);
    vector_t *myPtr2 = vector_allocate_dynamic(dim);
    vector_t *myPtr3 = vector_allocate_dynamic(dim);

    vector_t *ptr_active = NULL;
    vector_t *ptr_active2 = NULL;
    vector_t *ptr_active_sum = NULL;


    vec_array_t *data_array = vec_array_load("input.txt");
    if(data_array ==NULL){
        fprintf(stderr, SPACER_H);
        fprintf(stderr, "\nLoading input data for array failed. Place input.txt in root folder\n");
    }

    printf("%s\n\t\t\t%s\n%s\n", SPACER_H, "Dynamic Memory Allocation", SPACER_H);

    printf("\nAllocated 1 vector array for you.\n"
           "Allocated 3 vectors for you. You can choose between '1' , '2' and '3'.\n"
           "Freeing the memory will be accomplished for you after quitting the main menu.\n");

    while(!flag_terminate){
        if (response != SUCCESS){
            switch (response){
                case NULL_POINTER_ERROR: fprintf(stderr,"NULL POINTER ERROR: Null pointer error occurred. \n"); break;
                case ALLOC_FAILURE_ERROR: fprintf(stderr,"ALLOC FAILURE ERROR: Allocation failure occurred. \n"); break;
                case OUT_OF_RANGE_ERROR: fprintf(stderr,"OUT OF RANGE ERROR: Invalid index stated. Check if index is negative or simply or out range.\n"); break;
                case DIMENSION_MISMATCH_ERROR: fprintf(stderr,"DIM MISMATCH ERROR: Dimensions of vectors does not match.\n"); break;
                case SUCCESS: fprintf(stderr, "SUCCESS: All operations successfully executed.\n"); break;
                default: fprintf(stderr, "BUG: Impossible to be here!\n");break;
            }
        }
        printf("Options: \n"
           "\t1 - Print vector\n"
           "\t2 - Change dimension of vector\n"
           "\t3 - Fill vector\n"
           "\t4 - Add two vectors\n"
           "\t5 - Slice a vector\n"
           "\t6 - Store vector in array\n"
           "\t7 - Delete vector in array\n"
           "\t8 - Save vector array\n"
           "\t9 - Load vector array\n"
           "\t10 - Sort by Norm\n"
           "\t0 - End program\n");

        menu_ctrl = readValue("Selected");
        fflush(stdin);
        switch(menu_ctrl){
            case 0: flag_terminate = 1; break;
            case 1:
                ptr_active = choose_vec(myPtr, myPtr2, myPtr3, data_array);
                response = vector_print(ptr_active);
                break;
            case 2:
                ptr_active = choose_vec(myPtr, myPtr2, myPtr3, data_array);
                response = vector_change_dim(ptr_active, readValue("Dimension"));
                break;
            case 3:
                ptr_active = choose_vec(myPtr, myPtr2, myPtr3, data_array);
                response = vector_fill(ptr_active,readValue("Index"), readValue("Value")); break;
                break;
            case 4:
                printf("Which vector is the first adding term?\n");
                ptr_active = choose_vec(myPtr, myPtr2, myPtr3, data_array);
                printf("Which vector is the second adding term?\n");
                ptr_active2 = choose_vec(myPtr, myPtr2, myPtr3, data_array);
                printf("Which vector is the sum?\n");
                ptr_active_sum = choose_vec(myPtr, myPtr2, myPtr3, data_array);
                response = vector_add(ptr_active,ptr_active2, ptr_active_sum);
                break;
            case 5:
                ptr_active = choose_vec(myPtr, myPtr2, myPtr3, data_array);
                response = vector_slice(ptr_active, readValue("Index start"), readValue("Index end"));
                break;
            case 6:
                printf("Which vector do you want to store in the array?\n");
                ptr_active = choose_vec(myPtr, myPtr2, myPtr3, data_array);
                response = vec_array_store(data_array, ptr_active);
                break;
            case 7:
                printf("Which vector do you want to delete from the array?\n"
                       "You can choose an index between 0 and %ld", data_array->length-1);
                menu_ctrl = readValue("Index");
                response = vec_array_delete_at(data_array, menu_ctrl);
                break;
            case 8:
                printf("Choose a file name. Recommanded file format: .txt\nName:\t");
                memset(&filename[0], 0, sizeof(filename));
                scanf("%s", filename);
                fflush(stdin);
                response = vec_array_save(data_array, filename);
                break;
            case 9:
                printf("Enter the name of the file you want to import.\n"
                       "WARNING: Old array will be lost!\nName:\t");
                memset(&filename[0], 0, sizeof(filename));
                scanf("%s", filename);
                fflush(stdin);
                response = vec_array_free(data_array);
                data_array = vec_array_load(filename);
                break;
            case 10:
                response = vec_array_sort(data_array, vector_norm);
                break;
            default: fprintf(stderr, "ERROR: This Option does not exist.\n");
        }
    }
    printf("\n=================END================\n\n");

    response = vector_free(myPtr);
    response = vector_free(myPtr2);
    response = vector_free(myPtr3);

    response = vec_array_free(data_array);
}





