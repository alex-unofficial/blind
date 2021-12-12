#include "helpers.h"

int binom(int n, int k) {
    if(k == 0 || k == n) return 1;
    
    k = (n - k < k)? n - k : k;

    float prod = 1;
    for(int i = 1 ; i <= k ; i++) {
        prod *= (float)(n + 1 - i)/i;
    }

    return (int)prod;
}

int fround(float x) {
    return (int) (2 * x) - (int) x;
}