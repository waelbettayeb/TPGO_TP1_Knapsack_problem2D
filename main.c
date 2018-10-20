#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
/********************/

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

static unsigned int MAXIMAL_VOLUME = 347;//cm


/********************/


void printSolution(Solution* sol, unsigned int numberOfObjects){
    printf("\nPrinting the solution ...\n");
    for(unsigned int i = 0; i < numberOfObjects; i++) {
        printf("| \t%d \t|\n", sol->X[i]);
    }
    printf("DONE\n");
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
Solution* cloneSolution(Solution* sol){
    Solution *solC = malloc(sizeof(Solution));
    solC->value = sol->value;

    solC->X = malloc(data->numberOfObjects*sizeof(bool));
    for(unsigned int i = 0; i < data->numberOfObjects ;i++) {
        solC->X[i] = sol->X[i];
    }

    return solC;
};
struct _SolutionMatrix{
    Solution***tab;
}typedef MatrixSolution;
Solution* fDynamique(unsigned int n,unsigned int maxWeight, unsigned int maxVolume) {

    MatrixSolution tabF[n+1];
    Solution *f1;
    Solution *f2;
    for (unsigned int i = 0; i < n + 1; i++) {
        tabF[i].tab = malloc((maxWeight+1)* sizeof(Solution));
        for (unsigned int j = 0; j < maxWeight + 1; j++) {
            tabF[i].tab[j] = malloc((maxVolume+1)* sizeof(Solution));
            for(unsigned int k = 0; k < maxVolume + 1; k++) {
                if (i == 0) {
                    tabF[i].tab[j][k] = initSolution(data->numberOfObjects);
                } else {
                    //On suppose que l'object i n'est pas pris
                    if ((j < data->objects[i - 1].weight) ||(k < data->objects[i - 1].volume)){
                        tabF[i].tab[j][k] = cloneSolution(tabF[i - 1].tab[j][k]);
                    } else {
                        //On suppose que l'object i est pris
                        f1 = cloneSolution(tabF[i - 1].tab[j][k]);
                        f2 = cloneSolution(tabF[i - 1].tab
                                           [j - data->objects[i - 1].weight]
                                           [k - data->objects[i - 1].volume]);
                        f2->value = f2->value + (data->objects[i - 1].value);
                        if (f1->value >= f2->value) {
                            tabF[i].tab[j][k] = f1;
                            free(f2);
                        } else {
                            tabF[i].tab[j][k] = f2;
                            tabF[i].tab[j][k]->X[i - 1] = true;
                            free(f1);
                        }
                    }
                }
            }
        }
    }
    return tabF[n].tab[maxWeight][maxVolume];
}

int main() {

    Solution* optimalSolution;

    data = malloc(sizeof(Data));
    data->numberOfObjects = NUMBER_OF_OBJECTS;
    data->objects = &obj;

    optimalSolution = f(NUMBER_OF_OBJECTS, MAXIMAL_WEIGHT, MAXIMAL_VOLUME);
    printSolution(optimalSolution, NUMBER_OF_OBJECTS);
    printf("\nThe optimal value = %u ", optimalSolution->value);

    optimalSolution = fDynamique(NUMBER_OF_OBJECTS, MAXIMAL_WEIGHT, MAXIMAL_VOLUME);
    printSolution(optimalSolution, NUMBER_OF_OBJECTS);
    printf("\nThe optimal value = %u ", optimalSolution->value);

    free(data);

}
