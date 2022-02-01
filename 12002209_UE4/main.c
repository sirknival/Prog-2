#include "functions.h"
//#define DEV
int main()
{
    int flag_terminate = 0;
    long menu_ctrl = 0;
    long status = 0;

    heap_t *heap_music = NULL;
    hash_t *music_hash = NULL;
    entry_t *search_result = NULL;
    char songtitle[TXT_LEN];
    char interpreter[TXT_LEN];
    char *ptr_song = &songtitle[0];
    char *ptr_interp = &interpreter[0];

    heap_music = heap_create(HEAP_ELEM);
    printf("MUSIC LIST MANAGEMENT TOOL\n");
    printf("Options: \n"
               "\t0 - End program\n"
               "\t1 - Delete heap - Create Heap\n"
               "\t2 - Add new entry in heap\n"
               "\t3 - Print heap\n"
               "\t4 - Search in Heap\n"
               "\t5 - Delete first entry in heap\n"
               "\t6 - Create Hash from heap\n"
               "\t7 - Delete entire hash\n"
               "\t8 - Add new entry to hash\n"
               "\t9 - Print hash\n"
               "\t10 - Search entry in hash\n"
               "\t11 - Delete entry from hash\n"
               "\t12 - Benchmark\n"
               "\t13 - \n"
               "\t14 - \n");

    while(!flag_terminate){
        menu_ctrl = readValue("Select:");
        fflush(stdin);
        switch(menu_ctrl){
            case 0:
                if(!heap_music){
                    if(!music_hash)
                        hash_free(music_hash);
                    flag_terminate = 1;
                }

                else
                    printf("Please empty the heap first!\n");
                break;
            case 1:
                if(heap_music){
                    heap_free(heap_music);
                    heap_music = NULL;
                }
                else
                    heap_music = heap_create(HEAP_ELEM);
                break;
            case 2:
                my_getline(ptr_song, TXT_LEN, "Songtitle");
                my_getline(ptr_interp, TXT_LEN, "Interpreter");
                status = heap_insert(heap_music, ptr_song, ptr_interp);
                if(status == 0)
                    fprintf(stderr, "ERROR: Inserting was not successful!\n");
                break;
            case 3:
                heap_print(heap_music);
                break;
            case 4:
                my_getline(ptr_song, TXT_LEN, "Songtitle");
                my_getline(ptr_interp, TXT_LEN, "Interpreter");
                status = heap_search(heap_music, ptr_song, ptr_interp);
                if(status == 0)
                    fprintf(stderr, "ERROR: Search was not successful!\n");
                else{
                    printf("Found element on index %ld\n", status);
                    print_entry(heap_music, status);
                }
                break;
            case 5:
                heap_delete_first(heap_music);
                break;
            case 6:
                music_hash = convert_heap_to_hash(heap_music);
                break;
            case 7:
                hash_free(music_hash);
                music_hash = NULL;
                break;
            case 8:
                my_getline(ptr_song, TXT_LEN, "Songtitle");
                my_getline(ptr_interp, TXT_LEN, "Interpreter");
                hash_insert(music_hash, ptr_song, ptr_interp);
                break;
            case 9:
                hash_print(music_hash);
                break;
            case 10:
                my_getline(ptr_song, TXT_LEN, "Songtitle");
                my_getline(ptr_interp, TXT_LEN, "Interpreter");
                search_result = hash_search(music_hash, ptr_song, ptr_interp);
                if(search_result)
                    printf("%s - %s\n",search_result->songtitle, search_result->interpreter);
                break;
            case 11:
                my_getline(ptr_song, TXT_LEN, "Songtitle");
                my_getline(ptr_interp, TXT_LEN, "Interpreter");
                hash_delete_entry(music_hash, ptr_song, ptr_interp);
                break;
            case 12:
                benchmark();
                break;
            case 13:
                break;
            case 14:
                break;
            default: break;
        };
    }
    printf("\n=================END================\n\n");

    return 0;
}
