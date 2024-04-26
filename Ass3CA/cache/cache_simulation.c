/*336165147 Tamar Hessen*/
#include <stdio.h>
#include "cache.h"
#include <stdlib.h>

// Function prototypes
cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E);
uchar read_byte(cache_t cache, uchar* start, long int off);
void write_byte(cache_t cache, uchar* start, long int off, uchar new);
void print_cache(cache_t cache);
char *long_to_binary(long n);
long int find_value(const char* binary_num, int b);

int main() {
     int n;
     printf("Size of data: ");
     scanf("%d", &n);
     uchar* mem = malloc(n);
    printf("Input data >> ");
    for (int i = 0; i < n; i++)
         scanf("%hhd", mem + i);

     int s, t, b, E;
     printf("s t b E: ");
     scanf("%d %d %d %d", &s, &t, &b, &E);
     cache_t cache = initialize_cache(s, t, b, E);

     while (1) {
         scanf("%d", &n);
         if (n < 0) break;
         read_byte(cache, mem, n);
         }

     puts("");
     print_cache(cache);
     free(mem);
     }

void print_cache(cache_t cache) {
     int S = 1 << cache.s;
     int B = 1 << cache.b;

     for (int i = 0; i < S; i++) {
         printf("Set %d\n", i);
         for (int j = 0; j < cache.E; j++) {
             printf("%1d %d 0x%0*lx ", cache.cache[i][j].valid,
                      cache.cache[i][j].frequency, cache.t, cache.cache[i][j].tag);
             for (int k = 0; k < B; k++) {
                 printf("%02x ", cache.cache[i][j].block[k]);
                 }
             puts("");
             }
         }
     }

cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E) {
    cache_t cache;
    cache.s = s;
    cache.t = t;
    cache.b = b;
    cache.E = E;
    cache.cache = (cache_line_t**)malloc((1 << cache.s) * sizeof(cache_line_t*)); // Corrected calculation for the number of sets
    if (cache.cache == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < (1 << cache.s); i++) {
        cache.cache[i] = (cache_line_t*)malloc(cache.E * sizeof(cache_line_t));
        if (cache.cache[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        for (int j = 0; j < cache.E; j++) {
            cache.cache[i][j].valid = 0;
            cache.cache[i][j].frequency = 0;
            cache.cache[i][j].tag = 0;
            // Allocate memory for the block and set all bits to 0
            cache.cache[i][j].block = (uchar*)calloc((1 << cache.b), sizeof(uchar)); // Corrected calculation for the number of blocks
            if (cache.cache[i][j].block == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
        }
    }
    return cache;
}
uchar read_byte(cache_t cache, uchar* start, long int off) {
    if(off==123){
        int h=0;
    }
    long int place = off;
    int s = cache.s ;
    int b = cache.b ;
    int t = cache.t;
    int block_place = off & ((1 << b) - 1);
    off = off >> b;

    int set_place = off & ((1 << s) - 1);
    off = off >> s;

    int tag_place = off;
    int min_freq_index = 0;
    int min_freq = cache.cache[set_place][0].frequency;

    for (int i = 0; i < cache.E; i++) {
        if (cache.cache[set_place][i].tag == tag_place && cache.cache[set_place][i].valid == 1) {
            cache.cache[set_place][i].frequency++;
            cache.cache[set_place][i].tag = tag_place;
            return cache.cache[set_place][i].block[block_place];
        }
        if (cache.cache[set_place][i].frequency < min_freq) {
            min_freq = cache.cache[set_place][i].frequency;
            min_freq_index = i;
        }
    }
    if(cache.cache[set_place][min_freq_index].valid == 1){
        cache.cache[set_place][min_freq_index].tag = tag_place;
        cache.cache[set_place][min_freq_index].frequency =1;
        cache.cache[set_place][min_freq_index].block[block_place] = start[place];
        int e=1<<cache.b;
        for (int j = 0; j < e; j++) {
            cache.cache[set_place][min_freq_index].block[j] = start[place - block_place +j];
        }
    }

    if (cache.cache[set_place][min_freq_index].valid == 0) {
        cache.cache[set_place][min_freq_index].valid = 1;
        cache.cache[set_place][min_freq_index].frequency = 1;
        cache.cache[set_place][min_freq_index].tag = tag_place;
        cache.cache[set_place][min_freq_index].block[block_place] = start[place];
        int d=1<<cache.b;
        for (int j = 0; j < d; j++) {
            int z=start[place - block_place +j];
            cache.cache[set_place][min_freq_index].block[j] = start[place - block_place +j];
        }
    }
    return cache.cache[set_place][tag_place].block;
}



void write_byte(cache_t cache, uchar* start, long int off, uchar new) {
    long int place = off;
    int s = cache.s;
    int b = cache.b;
    int t = cache.t;
    int block_place = off & ((1 << b) - 1);
    off = off >> b;

    int set_place = off & ((1 << s) - 1);
    off = off >> s;

    int tag_place = off;

    if (cache.cache[set_place][tag_place].valid == 1) {
        cache.cache[set_place][tag_place].block[block_place] = new;
        cache.cache[set_place][tag_place].frequency = 1;
    } else {
        start[place] = new;
        cache.cache[set_place][tag_place].frequency = 1;
        cache.cache[set_place][tag_place].tag = tag_place;
        for (int j = 0; j < b; j++) {
            cache.cache[set_place][tag_place].block[j] = start[place - block_place + j];
        }
    }
}


