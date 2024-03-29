#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <omp.h>
#include "timing.h"
#include "debugbmp.h"

size_t pos(size_t width, size_t x, size_t y) {
    size_t result = width*y + x;
    return result;
}

void write_borders(float* data, size_t width, size_t height) {
    #pragma omp parallel for schedule(static) // schedule included for readbility. It is default by OpenMP.
    for (size_t n = 0; n < width ; n ++){ 
        data[pos(width, n, 0)] = 20; 
        data[pos(width, n, height-1)] = -273.15;
    }
    #pragma omp parallel for schedule(static)
    for ( size_t n = 0; n < height ; n++){ 
        data[pos(width, 0, n)] = -273.15;
        data[pos(width, width-1, n)] = -273.15;
    }
}

float stencil(float* data, size_t width, size_t x, size_t y, float alpha) {
    return (alpha * (data[pos(width, x,y)] + data[pos(width, x-1,y)] + data[pos(width, x+1,y)] + data[pos(width, x,y-1)] + data[pos(width, x,y+1)]));
}

void apply_stencil(float* data, float* prev, size_t width, size_t height, size_t offset, float alpha) {
    #pragma omp parallel for collapse(2)
    for (size_t x = 1; x < (width-1); x++) {
        for (size_t y = 1; y < (height-1); y++) {
            if ((x + y + offset) % 2 == 0) {  // Process either even or odd (x + y) based on offset
                data[pos(width, x, y)] = stencil(prev, width, x, y, alpha);
            }
        }
    }
}


float compute_delta(float* data, float* prev, size_t width, size_t height) {
    float res = 0.0; 

    #pragma omp parallel for collapse(2) reduction(+:res)
    for( size_t x= 0; x < width ; x++){ 
        for(size_t y = 0; y < height ; y++){
            res = res + fabs(prev[pos(width, x, y)] - data[pos(width, x, y)]);
            
        }
    }

    return (res / (width * height));
}

void run_simulation(size_t width, size_t height, size_t steps, const char* filename) {
    double bef = seconds();
    size_t size = width*height;

    float* data = malloc(size * sizeof(float));
    float* prev = malloc(size * sizeof(float));

    memset(data, 0, size * sizeof(float));
    write_borders(data, width, height);

    float delta = 0.0f;
    size_t n = 0;


    double start_time = seconds();
    for(; n < steps; n++) {
        memcpy(prev, data, size*sizeof(float));
        apply_stencil(data, prev, width, height, n % 2, 0.2f);
        delta = compute_delta(data, prev, width, height);
        if (delta < 0.001f)
            break;
    }
    double end_time = seconds();
    double par_time = end_time - start_time;
    printf("Parallelized time: %f\n", (par_time));


    if (filename != NULL) {
        debugbmp_writebmp(filename, (int)width, (int)height, data);
    }



    free(data);
    free(prev);

    //timings and prints seq+overall+delta
    double aft = seconds();
    double seq_time = (aft - bef) - par_time;
    printf("Sequential time: %f\n", seq_time);
    printf("Total time: %f\n", (aft - bef));
    printf("After %lu iterations, delta was %f\n", n, delta);
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