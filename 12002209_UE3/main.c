#include "operations.h"

int main()
{
    int flag_terminate = 0;
    long menu_ctrl = 0;

    node_t *music_tree = NULL;
    node_t *matched_node = NULL;
    char *ptr_song = calloc(TXTLEN, sizeof(char));
    char *ptr_singer = calloc(TXTLEN, sizeof(char));

    node_t **ptr_search_result = NULL;
    long found_number = 0;

    printf("MUSIC LIST MANAGEMENT TOOL\n");
    printf("Options: \n"
               "\t0 - End program\n"
               "\t1 - Delete entire tree\n"
               "\t2 - Add new Entry to tree\n"
               "\t3 - Print tree\n"
               "\t4 - Count Nodes\n"
               "\t5 - Find first matching node with songtitle\n"
               "\t6 - Insert new entry at index x\n"
               "\t7 - Print max. tree depth\n"
               "\t8 - Print specified level\n"
               "\t9 - Print in random order\n"
               "\t10 - Print average compasrisons\n"
               "\t11 - Rebalance tree\n"
               "\t12 - Benchmarking\n"
               "\t13 - Print tree as list (format inorder)\n"
               "\t14 - Validate binary tree proprity\n");

    while(!flag_terminate){
        if(music_tree != NULL)
            music_tree = tree_top(music_tree);
        menu_ctrl = readValue("Select:");
        fflush(stdin);
        switch(menu_ctrl){
            case 0:
                if(music_tree == NULL)
                    flag_terminate =1;
                else
                    fprintf(stderr, "ERROR: Please free the list before you quit!\n");
                break;
            case 1:
                destroy_tree(music_tree);
                music_tree = NULL;
                break;
            case 2:
                my_getline(ptr_song, TXTLEN, "Songtitle");
                my_getline(ptr_singer, TXTLEN, "Interpreter");
                music_tree = insert_node(music_tree, ptr_song, ptr_singer);
                break;
            case 3:
                print_tree(music_tree);
                break;
            case 4:
                printf("Number of nodes in this tree: %ld\n", count_nodes(music_tree));
                break;
            case 5:
                printf("Enter the songtitle to query for\n");
                my_getline(ptr_song, TXTLEN, "Songtitle");
                matched_node = search_node(music_tree, ptr_song);
                if(matched_node != NULL)
                    printf("%s - %s\n", matched_node->songtitle, matched_node->interpreter);
                else
                    printf("Nothing found!\n");
                break;
            case 6:
                my_getline(ptr_singer, TXTLEN, "Interpreter");
                ptr_search_result = search_all(music_tree, ptr_singer, &found_number);
                printf("Results:\n");
                for (int j = 0; j < found_number; j++)
                    printf(">>  %s - %s\n", (*(ptr_search_result+j))->songtitle, (*(ptr_search_result+j))->interpreter );
                free(ptr_search_result);
                found_number = 0;
                break;
            case 7:
                printf("Max depth of tree: %ld\n", max_depth(music_tree));
                break;
            case 8:
                menu_ctrl = readValue("Level");
                if(menu_ctrl < max_depth(music_tree))
                    print_level(music_tree, menu_ctrl);
                else
                    fprintf(stderr, "ERROR: Level higher than max depth!\n");
                break;
            case 9:
                my_getline(ptr_song, TXTLEN, "Songtitle");
                my_getline(ptr_singer, TXTLEN, "Interpreter");
                music_tree = delete_node(music_tree, ptr_song, ptr_singer);
                break;
            case 10:
                printf("Average comparision: %f\n", average_comparisons(music_tree));
                break;
            case 11:
                rebalance_tree(music_tree);
                break;
            case 12:
                benchmark();
                break;
            case 13:
                print_inorder(music_tree);
                break;
            case 14:
                printf("Check result: %ld\n", check_if_order_OK(music_tree));
                printf("1 == ERROR ; 0 == SUCCESS\n");
                break;
            default: break;
        };
    }
    printf("\n=================END================\n\n");

    free(ptr_singer);
    free(ptr_song);
    return 0;
}
