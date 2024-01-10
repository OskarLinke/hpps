#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <omp.h>
#include "debugbmp.h"

size_t pos(size_t width, size_t x, size_t y) {
    size_t result = width*y + x;
    return result;
}

void write_borders(float* data, size_t width, size_t height) {
    for (n = 0; n ++; n < width){ 
        data[pos(width, n,0)] = 20; 
        data[pos(width, n,height-1)] = -273.15;
    }

    for ( n = 0; n++; n < height){ 
        data[pos(width, 0,n)] = -273.15
        data[pos(width, width-1,n)] = -273.15
    }
}

float stencil(float* data, size_t width, size_t x, size_t y, float alpha) {
    assert(0);
}

void apply_stencil(float* data, size_t width, size_t height, size_t offset, float alpha) {
    assert(0);
}

float compute_delta(float* data, float* prev, size_t width, size_t height) {
    float res = 0.0; 

    for( x= 0; x++; x < width){ 
        for(y = 0; y++; y < height){
            res = res + fabs(prev[pos(width, x, y)]-data[pos(width, x, y)]);
            
        }
    }

    return (res / (width * height));
}

void run_simulation(size_t width, size_t height, size_t steps, const char* filename) {
    size_t size = width*height;

    float* data = malloc(size * sizeof(float));
    float* prev = malloc(size * sizeof(float));

    memset(data, 0, size * sizeof(float));
    write_borders(data, width, height);

    float delta = 0.0f;
    size_t n = 0;

    for(; n < steps; n++) {
        memcpy(prev, data, size*sizeof(float));
        apply_stencil(data, width, height, n % 2, 0.2f);
        delta = compute_delta(data, prev, width, height);
        if (delta < 0.001f)
            break;
    }

    printf("After %lu iterations, delta was %f\n", n, delta);
    if (filename != NULL) {
        debugbmp_writebmp(filename, (int)width, (int)height, data);
    }

    free(data);
    free(prev);
}

int main(int argc, char** argv) {
    if (argc != 4 && argc != 5) {
        fprintf(stderr, "Usage: %s <width> <height> <steps> [output-file]\n", argv[0]);
        return 1;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int steps = atoi(argv[3]);

    if (width <= 0 || height <= 0) {
        fprintf(stderr, "Sizes must be positive integers\n");
        return 1;
    }

    if (steps < 0) {
        fprintf(stderr, "Steps must be non-negative\n");
        return 1;
    }

    char* filename = NULL;
    if (argc == 5) {
        filename = argv[4];
    }

    run_simulation((size_t)width, (size_t)height, (size_t)steps, filename);

    return 0;
}