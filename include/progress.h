#ifndef PROGRESS_H
#define PROGRESS_H
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "ansi_colors.h"

#define MAX_LENGTH   50
#define PERCENTAGE(V, T) (MAX_LENGTH - (((T - V) * MAX_LENGTH) / T))



// Get the current time in milliseconde
static inline double walltime_now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}



static inline int calculate_percentage(size_t count, size_t max) {
    return (max > 0) ? (int)((100.0 * count) / (double)max) : 0;
}



// Display progress bar and update frequency 
void print_progress(size_t count, size_t max)
{
    static double last_update_t = 0.0;
    static int last_percent = -1;
    static int initialized = 0;

    if (!initialized) {
        setvbuf(stdout, NULL, _IONBF, 0); // disable the buffering 
        initialized = 1;
        last_update_t = walltime_now();
    }

    double now = walltime_now();
    double dt = now - last_update_t;

    // Update period 
    const double UPDATE_PERIOD = 0.05;
    if (count < max && dt < UPDATE_PERIOD)
        return;

    last_update_t = now;

    int pp = calculate_percentage(count, max);
    if (pp == last_percent && count < max)
        return;
    last_percent = pp;

    char buffer[MAX_LENGTH + 16];
    buffer[0] = '[';
    for (int i = 0; i < MAX_LENGTH; ++i)
        buffer[i + 1] = (   (int)i  < (int) PERCENTAGE(count, max)) ? '#' : ' ';
    buffer[MAX_LENGTH + 1] = ']';
    buffer[MAX_LENGTH + 2] = '\0';

    if (count < max) {
        printf("\r%c[2K %s %sProgress %3d%%%s  (%zu / %zu rows)",
               27, buffer, BCYN, pp, reset, count, max);
    } else {
        printf("\r%c[2K %s %sComplete %3d%%%s  (Total: %zu rows)\n",
               27, buffer, BGRN, pp, reset, max);
    }

    fflush(stdout);
}

#endif  

