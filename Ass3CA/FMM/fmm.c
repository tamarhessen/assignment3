/* 336165147 Tamar Hessen */;
#include "fmm.h"
#include <stdio.h>
#include <stdlib.h>

void fmm(int n, int* m1, int* m2, int* result) {
    int* col = malloc(n * sizeof(int));
    
    if (col == NULL) {
        return;
    }
    
    for(register int j = 0; j < n; j++) {
        for(register int k = 0; k < n; k++) {
            col[k] = m2[k * n + j];
        }
        
        for(register int i = 0; i < n; i++) {
            register int ilen = i * n;
            register int s = 0;
            for(register int k = 0; k < n; k++) {
                s += m1[ilen + k] * col[k];
            }
            result[ilen + j] = s;
        }
    }
    
    free(col);
}
