#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define SIZE_POP 10
#define PCT_MUTATION 2
#define START_INTERVAL_SIZE 200

// Define function here
double function(double x) {
    double y;

    y = x*x-x-2; // Root = -1 and 2
    return y;
}

void start_population(double **p) {
    int i;

    *p = (double *) malloc(sizeof(double) * SIZE_POP);
    for (i = 0; i < SIZE_POP; i++) {
        (*p)[i] = (double) ((rand()%START_INTERVAL_SIZE) - START_INTERVAL_SIZE/2);
        (*p)[i] += (double) ((rand()%101)/100.0);
    }
}

void print_population(double *p) {
    int i;

    for (i = 0; i < SIZE_POP; i++) {
        printf("Population[%d] = %.2lf\n", i, p[i]);
    }
}

int find_best(double* p) {
    int i, best_index;
    double best_value, current;

    best_index = 0;
    best_value = function(p[0]);
    for (i = 1; i < SIZE_POP; i++) {
        current = function(p[i]);
        if (fabs(current) < fabs(best_value)) {
            best_value = current;
            best_index = i;
        }
    }
    return best_index;
}

void crossover(double* p, int best_index) {
    int i;
    double best = p[best_index];
    double mutated;

    for (i = 0; i < SIZE_POP; i++) {
        p[i] = (best + p[i]) / 2.0;
        mutated = p[i] + ((double) (rand()%START_INTERVAL_SIZE)-START_INTERVAL_SIZE/2)*PCT_MUTATION/100.0;
        if (i != best_index) p[i] = mutated;
    }
}

int main(int argc, char *argv) {
    int best;        // best individual index
    int generation = 0; // generation counter
    char exit = 0;
    double *population;  // stores the individuals

    start_population(&population);
    printf("Initial Population:\n");
    print_population(population);

    srand(time(NULL));  // initializes the random seed

    printf("Simulator to find at least one root of the equation f(x)=x^2-x-2\n");
    printf("The roots are x=-1 and x=2\n");
    printf("Press 'q' to quit or any other key to continue\n");

    while(exit != 'q') {
        best = find_best(population); // Finds the best individual in the population
        printf("Generation %d\tBest x = %.2lf\tf(x)=%.2lf", generation++,
                population[best], function(population[best]));
        crossover(population, best); // Mutates using crossover

        exit = getchar();
    }
    return 0;
}
