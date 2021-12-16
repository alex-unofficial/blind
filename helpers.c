#include "helpers.h"

/* The binomial coefficient formula is given by
 * n!/k!(n-k)!
 *
 * the function below computes exactly this
 */
int binom(int n, int k) {
    // both binom(0,n) and binom(n,n) are equal to 1
    if(k == 0 || k == n) return 1;
    
    // binom(n, k) is equal to binom(n, n-k) so we choose the lowest of the 2
    k = (n - k < k)? n - k : k;

    // computes the formula above
    float prod = 1;
    for(int i = 1 ; i <= k ; i++) {
        prod *= (float)(n + 1 - i)/i;
    }

    return (int)prod;
}

// returns the closest integer to the double x.
int dround(double x) {
    return (int) (2 * x) - (int) x;
}
