#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define max(A,B) ((A>B) ? (A):(B))
#define min(A,B) ((A<B) ? (A):(B))

#define SIZE_POP 10
#define PCT_MUTATION 2
#define START_INTERVAL_SIZE 200
#define ELITISM 1
#define TOURNAMENT 2
#define SELECTION_METHOD 2  // 1: Elitism
                            // 2: Tournament of 2 selection
                            // Anything else: Elitism
#define GENOCIDE_COUNTDOWN 30

// Define function here
double function(double x) {
    double y;

    y = x*x-x-2; // Root = -1 and 2
    return y;
}

double generate_individual() {
    double x = (double) ((rand()%START_INTERVAL_SIZE) - START_INTERVAL_SIZE/2);
    return (x + (double) ((rand()%101)/100.0));
}

void start_population(double **p) {
    int i;

    *p = (double *) malloc(sizeof(double) * SIZE_POP);
    for (i = 0; i < SIZE_POP; i++) {
        (*p)[i] = generate_individual();
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

double mutate(double x) {
    return x + ((double) (rand()%START_INTERVAL_SIZE)-START_INTERVAL_SIZE/2)*PCT_MUTATION/100.0;
}

void elitism(double* p, int best_index) {
    int i;
    double best = p[best_index];

    for (i = 0; i < SIZE_POP; i++) {
        p[i] = (best + p[i]) / 2.0;
        if (i != best_index) p[i] = p[i] = mutate(p[i]);
    }
}

void tournament(double *p, int best_index) {
    int i;
    double dad, mom, child;

    double *children; // Array to save the new generated individuals

    children = (double *) malloc(sizeof(double) * SIZE_POP);
    for(i = 0; i < SIZE_POP; i++){ //
        if (i != best_index) {  // Don't change the best individual
            // dad and mom can be the same individual (allows parthenogenesis)
            dad = max(p[rand()%SIZE_POP], p[rand()%SIZE_POP]);
            mom = max(p[rand()%SIZE_POP], p[rand()%SIZE_POP]);

            child = mutate((dad + mom) / 2); // Creates the new son
            children[i] = child; // Replaces him in the population
        }
        else {
            children[i] = p[best_index]; // The best individual is kept
        }
    }
    for(i = 0; i < SIZE_POP; i++){ // Replaces the individuals with their children
        p[i] = children[i];
    }
    free(children);

    return;
}

void genocide(double *p, int best_index) {
    int i;

    printf("Genocide in progress...\n");
    for (i = 0; i < SIZE_POP; i++) {
        if (i != best_index) {
            p[i] = generate_individual();
        }
    }
}

double calculate_mean_fit (double *p) {
    int i;
    double mean = 0.0;

    for (i = 0; i < SIZE_POP; i++) {
        mean = mean + p[i]/SIZE_POP;
    }
    return mean;
}

int main(int argc, char *argv) {
    int best;        // Best individual index
    // Genocide happens when the best doesn't change in x generations
    int previous_best, genocide_counter = 0;
    int generation = 0; // Generation counter
    char exit = 0;
    double *population;  // Stores the individuals
    FILE *output;

    output = fopen("fitness.txt", "w+");

    start_population(&population);
    printf("Initial Population:\n");
    print_population(population);

    srand(time(NULL));  // initializes the random seed

    printf("Simulator to find at least one root of the equation f(x)=x^2-x-2\n");
    printf("The roots are x=-1 and x=2\n");
    printf("Press 'q' to quit or any other key to continue\n");

    // For the genocide countdown
    best = find_best(population); // Finds the best individual before entering the loop

    fprintf(output, "mean\tbest\n");
    while(exit != 'q') {
        previous_best = best; // Defines the previous best as being the same
        best = find_best(population); // Finds the best individual in the population

        if (best == previous_best)
            genocide_counter++;
        else
            genocide_counter = 0;

        printf("Generation %d\tBest x = %.2lf\tf(x)=%.2lf\n", generation++,
                population[best], function(population[best]));

        fprintf(output, "%.2lf\t%.2lf\n", calculate_mean_fit(population), population[best]);

        if (genocide_counter == GENOCIDE_COUNTDOWN) { // Kills all but the best
            genocide(population, best);
            genocide_counter = 0;
        }
        else if (SELECTION_METHOD == ELITISM)
            elitism(population, best); // Mutates using crossover with the best individual
        else if (SELECTION_METHOD == TOURNAMENT)
            tournament(population, best); // Mutates using tournament selection
        else elitism(population, best);

        exit = getchar();
    }

    fclose(output);
    return 0;
}
