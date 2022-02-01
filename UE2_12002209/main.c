#include "functions.h"

int main()
{
    int flag_terminate = 0;
    long menu_ctrl = 0;

    Element_t *music_list = NULL;

    printf("MUSIC LIST MANAGEMENT TOOL\n");
    printf("Options: \n"
               "\t0 - End program\n"
               "\t1 - Append new entry at the end\n"
               "\t2 - Print list\n"
               "\t3 - Delete list\n"
               "\t4 - Count elements\n"
               "\t5 - Print element from index x\n"
               "\t6 - Insert new entry at index x\n"
               "\t7 - Delete item at index x\n"
               "\t8 - Print in reverse order\n"
               "\t9 - Print in random order\n"
               "\t10 - Sort list by ascending song titles\n"
               "\t11 - Sort list by descending song titles\n"
               "\t12 - Remove duplicates from list\n"
               "\t13 - Rotate list by x\n");

    while(!flag_terminate){
        menu_ctrl = readValue("Select:");
        fflush(stdin);
        switch(menu_ctrl){
            case 0:
                if(music_list == NULL)
                    flag_terminate =1;
                else
                    fprintf(stderr, "ERROR: Please free the list before you quit!\n");
                break;
            case 1: music_list = insert_last(music_list); break;
            case 2: print_entire_list(music_list); break;
            case 3: free_list(&music_list); break;
            case 4: printf("Number of elements in list: %ld\n",elements_count(music_list)); break;
            case 5:
                menu_ctrl = readValue("Index");
                print_single_element(element_at(music_list, menu_ctrl)); break;
            case 6:
                menu_ctrl = readValue("Index");
                music_list = insert_at(music_list, menu_ctrl);
                break;
            case 7:
                menu_ctrl = readValue("Index");
                music_list = delete_at(music_list, menu_ctrl);
                break;
            case 8: print_list_reverse(music_list); break;
            case 9: print_list_random(music_list); break;
            case 10: music_list = sort_list(music_list, compare_songtitles_larger); break;
            case 11: music_list = sort_list(music_list, compare_songtitles_smaller); break;
            case 12: music_list = unique_list(music_list); break;
            case 13:
                menu_ctrl = readValue("Shift");
                music_list = rotate_list(music_list, menu_ctrl);
                break;
            default: break;
        };
    }
    printf("\n=================END================\n\n");
    return 0;
}
