#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
/********************/

clock_t begin, end;
double time_spent;

struct _Item{
    unsigned int weight;
    unsigned int volume;
    unsigned int value;
}typedef Item;

struct _Data{
    unsigned int numberOfObjects;
    Item* objects;
}typedef Data;

struct _Solution{
    bool *X;
    unsigned int value;
}typedef Solution;

struct _SolutionMatrix{
    Solution***tab;
}typedef MatrixSolution;


/*******************/

Item obj[20] = {
        {20, 10, 06},
        {30, 15, 8},
        {50, 25, 14},
        {20, 10, 06},
        {40, 20, 13},
        {60, 30, 17},
        {30, 15, 10},
        {10, 05, 04},
        {14, 07, 05},
        {36, 18, 11},
        {72, 36, 26},
        {86, 43, 35},
        {05, 03, 02},
        {03, 02, 01},
        {07, 04, 02},
        {23, 12, 07},
        {49, 25, 15},
        {57, 29, 17},
        {69, 35, 30},
        {12, 06, 03}
};

Data *data;

static unsigned int NUMBER_OF_OBJECTS = 20;

static unsigned int MAXIMAL_WEIGHT = 520;//kg

static unsigned int MAXIMAL_VOLUME = 300;//m3 * 100

MatrixSolution tabF[20+1];


/********************/


void printSolution(Solution* sol, unsigned int numberOfObjects){
    printf("\nPrinting the solution ...\n");
    printf("\nThe optimal value = %u \n", sol->value);
    printf("The objects : ");
    for(unsigned int i = 0; i < numberOfObjects; i++) {
        //printf("| \t%d \t|\n", sol->X[i]);
        if(sol->X[i] == 1 ) printf("%d,",i+1);
    }
    printf("\n\n");
}
Solution* initSolution(unsigned int length){
    Solution *sol = malloc(sizeof(Solution));
    sol->X = calloc(length, sizeof(bool));
    sol->value = 0;
    return sol;

}
Solution* f(unsigned int n,unsigned int maxWeight, unsigned int maxVolume) {

    Solution *f1;
    Solution *f2;

    if (n <= 0)
        return initSolution(data->numberOfObjects);
    else {
        //On suppose que l'object i n'est pas pris
        f1 = f(n - 1, maxWeight, maxVolume);
        if((maxWeight < data->objects[n - 1].weight)||(maxVolume < data->objects[n - 1].volume))
            return f1;
        else{
            //On suppose que l'object i est pris
            f2 = f(n - 1, maxWeight - data->objects[n - 1].weight, maxVolume - data->objects[n - 1].volume);
            f2->value += data->objects[n - 1].value;
        }
        if (f1->value >= f2->value) {
            free(f2);
            return f1;
        } else {
            f2->X[n-1] = true;
            free(f1);
            return f2;
        }
    }
}
/********************************/
Solution* cloneSolution(Solution*solC, Solution* sol, unsigned int n){

    solC->value = sol->value;
    for(unsigned int i = 0; i < n ;i++) {
        solC->X[i] = sol->X[i];
    }

    return solC;
};

void mkarr(unsigned int n,unsigned int maxWeight, unsigned int maxVolume){
    for (unsigned int i = 0; i < n + 1; i++) {
        tabF[i].tab = malloc((maxWeight+1)* sizeof(Solution));
        for (unsigned int j = 0; j < maxWeight + 1; j++) {
            tabF[i].tab[j] = malloc((maxVolume+1)* sizeof(Solution));
            for(unsigned int k = 0; k < maxVolume + 1; k++) {
                    tabF[i].tab[j][k] = initSolution(data->numberOfObjects);
            }
        }
    }
}

Solution* fDynamique(unsigned int n,unsigned int maxWeight, unsigned int maxVolume) {

    Solution *f2;
    for (unsigned int i = 0; i < n + 1; i++) {
        for (unsigned int j = 0; j < maxWeight + 1; j++) {
            for(unsigned int k = 0; k < maxVolume + 1; k++) {
                if (i == 0) {
                } else {
                    if ((j < data->objects[i - 1].weight) ||(k < data->objects[i - 1].volume)){
                        tabF[i].tab[j][k] = tabF[i - 1].tab[j][k];
                    } else {
                        f2 = tabF[i - 1].tab
                                [j - data->objects[i - 1].weight]
                                [k - data->objects[i - 1].volume];
                        if ((tabF[i - 1].tab[j][k]->value) >=
                                (f2->value + (data->objects[i - 1].value))) {
                            //On suppose que l'object i n'est pas pris
                            tabF[i].tab[j][k] = tabF[i - 1].tab[j][k];
                        } else {
                            //On suppose que l'object i est pris
                            cloneSolution(tabF[i].tab[j][k], f2, i);
                            tabF[i].tab[j][k]->value = f2->value + data->objects[i - 1].value;
                            tabF[i].tab[j][k]->X[i - 1] = true;
                        }
                    }
                }
            }
        }
    }
    return tabF[n].tab[maxWeight][maxVolume];
}

int main() {

    clock_t begin, end;
    double time_spent;

    Solution* optimalSolution;

    data = malloc(sizeof(Data));
    data->numberOfObjects = NUMBER_OF_OBJECTS;
    data->objects = &obj;

    begin = clock();
    optimalSolution = f(NUMBER_OF_OBJECTS, MAXIMAL_WEIGHT, MAXIMAL_VOLUME);
    end = clock();
    printSolution(optimalSolution, NUMBER_OF_OBJECTS);
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Execution time = %f \n\n", time_spent);

    mkarr(NUMBER_OF_OBJECTS, MAXIMAL_WEIGHT, MAXIMAL_VOLUME);

    begin = clock();
    optimalSolution = fDynamique(NUMBER_OF_OBJECTS, MAXIMAL_WEIGHT, MAXIMAL_VOLUME);
    end = clock();

    printSolution(optimalSolution, NUMBER_OF_OBJECTS);
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Execution time = %f \n\n", time_spent);

    free(data);

}
