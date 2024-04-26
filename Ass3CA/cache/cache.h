/*336165147 Tamar Hessen*/
#ifndef CACHE_H
#define CACHE_H

typedef unsigned char uchar;

typedef struct cache_line_s {
    uchar valid;
    uchar frequency;
    long int tag;
    uchar* block;
} cache_line_t;

typedef struct cache_s {
    uchar s;
    uchar t;
    uchar b;
    uchar E;
    cache_line_t** cache;
} cache_t;

cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E);
uchar read_byte(cache_t cache, uchar* start, long int off);
void write_byte(cache_t cache, uchar* start, long int off, uchar new1);
void print_cache(cache_t cache);
char *long_to_binary(long n);
long int find_value(const char* binary_num, int b);
#endif /* CACHE_H */
