#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

int linear_search(int * array, int len, int element) {
    for (int i = 0; i < len; i++) {
        if (*array == element) {
            // found element - return index
            return i;
        }
        array++;
    }
    // element not found
    return -1;
}
// minor optimization: we know the element is in the array, so take advantage of that
int linear_search_1(int * array, int len, int element) {
    int *a = array;
    for(;;) {
        if (*a == element) {
            // found element - return index
            return array - a;
        }
        a++;
    }
    // element not found
    return -1;
}
// loop unrolling
int linear_search_2(int * array, int len, int element) {
    int *a = array;
    for(;;) {
        if (a[0] == element) return array - a;
        if (a[1] == element) return array - a;
        if (a[2] == element) return array - a;
        if (a[3] == element) return array - a;
        a += 4;
    }
    // element not found
    return -1;
}
int binary_search(int * array, int len, int element) {
    // assume the array is sorted
    int left = 0, right = len - 1, middle = (len - 1) / 2;
    while (left <= right) {
        if (array[middle] > element) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
        middle = (left + right) / 2;
    }
    // element not found
    return -1;
}
u_int64_t time_func(int(*func)(int*,int,int), int * array, int len, int * elements, int trials) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (int i = 0; i < trials; i++) {
        func(array, len, elements[i]);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    // number of nanoseconds
    return (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
}
int main(int argc, char** argv) {
    if (argc != 2) {
        printf("please specify a size n (2^n) -- eg: 4 for 2^4\n");
        return -1;
    }
    int n_max = atoi(argv[1]);
    
    for (int n = 0; n <= n_max; n++) {
        //printf("size = %d\n", n);

        // create the array
        int size =  (int) (pow((double) 2, (double) n));
        int * array = malloc(size * sizeof(n));
        for (int i = 0; i < size; i++) {
            array[i] = i; // trivially sorted
        }

        // compute random elements to search for
        int trials = 30;
        int elements[trials];

        for (int i = 0; i < trials; i++) {
            elements[i] = rand() % size;
        }

        // function pointers
        int (*linear)(int*,int,int);
        int (*binary)(int*,int,int);
        linear = linear_search_2;
        binary = binary_search;

        // compute mean time
        double total_time_linear = 0.0;
        double total_time_binary = 0.0;

        total_time_linear += time_func(linear, array, size, elements, trials);
        total_time_binary += time_func(binary, array, size, elements, trials);
        
        printf("%d,%f,%f\n", n,  total_time_linear / trials, total_time_binary / trials);
    }
}