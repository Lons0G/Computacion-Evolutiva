#include <string.h>
#define popsize 100  /* Population size         */
#define codesize 156 /* Number of bits used to represent an individual */
#define VAR_LENGTH 39
#define NUM_VARS 4

#include "genetico.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCION AUXILIAR PARA EXPORTAR EL INDIVIDUO CON EL MEJOR FITNESS DE CADA
// GENERACION
void exportar_convergencia(const char *filename, double mejor_fitness,
                           int generacion) {
  FILE *file;

  if (generacion == 0) {
    file = fopen(filename, "w");
    fprintf(file, "Generacion,Mejor_Fitness\n");
  } else {
    file = fopen(filename, "a");
  }

  if (file == NULL) {
    perror("Error al abrir el archivo de convergencia");
    return;
  }

  fprintf(file, "%d,%.6f\n", generacion, mejor_fitness);
  fclose(file);
}

int main() {
  struct individual *temp;
  float Rseed;

  cls();
  printf("Enter the mutation rate -------------------------> ");
  scanf("%f", &Pm);
  printf("Enter the probability of crossover --------------> ");
  scanf("%f", &Pc);
  printf("Enter the maximum number of generations ---------> ");
  scanf("%d", &Gmax);
  do {
    printf("Enter random number seed, 0.0 to 1.0 ------------> ");
    scanf("%f", &Rseed);
  } while ((Rseed < 0.0) || (Rseed > 1.0));

  /* Perform the previous memory allocation */

  memory_allocation();

  /* Initialize random number generator */

  randomize();

  nmutation = 0;
  ncross = 0;
  bestfit.fitness = 0.0;
  bestfit.generation = 0;

  /* Initialize the populations and report statistics */
  initialize_pop();
  statistics(oldpop);
  initial_report();

  for (gen = 0; gen < Gmax; gen++) /* Iterate the given number of generations */
  {

    /* Create a new generation */

    generation();

    /* Compute fitness statistics on new populations */
    statistics(newpop);
    if (bestfit.fitness < newpop[0].fitness) {
      memcpy(bestfit.chrom, newpop[0].chrom, codesize * sizeof(unsigned));
      bestfit.x1 = newpop[0].x1;
      bestfit.x2 = newpop[0].x2;
      bestfit.x3 = newpop[0].x3;
      bestfit.x4 = newpop[0].x4;
      bestfit.fitness = newpop[0].fitness;
      bestfit.generation = gen; // Actualizar la generación donde se encontró
    }
    /* Report results for new generation */

    report();

    /* Advance the generation */

    temp = oldpop;
    oldpop = newpop;
    newpop = temp;
  }

  printf("\n--- Resultado Final ---\n");
  printf("Mejor solución encontrada (generación %d):\n", bestfit.generation);
  printf("  Cadena binaria: ");
  writechrom(bestfit.chrom);
  printf("\n  Valores reales: x1=%.6f, x2=%.6f, x3=%.6f, x4=%.6f\n", bestfit.x1,
         bestfit.x2, bestfit.x3, bestfit.x4);
  printf("  Fitness: %.6f\n", bestfit.fitness);
  printf("  f(x) = %.6f\n", (1.0 / bestfit.fitness) - 1.0);

  free_all();
}

/* Define randomly the population for generation # 0 */

void initialize_pop() {
  int j, k;

  for (j = 0; j < popsize; j++) {
    // Asignar memoria para el cromosoma (156 bits)
    oldpop[j].chrom = (unsigned *)malloc(codesize * sizeof(unsigned));

    // Inicializar cromosoma con bits aleatorios (usando flip(0.5))
    for (k = 0; k < codesize; k++) {
      oldpop[j].chrom[k] =
          flip(0.5); // flip(0.5) devuelve 0 o 1 con 50% de probabilidad
    }

    // Decodificar cada variable (39 bits por variable)
    oldpop[j].x1 =
        decode(oldpop[j].chrom, 0 * VAR_LENGTH, VAR_LENGTH, -20.0, 20.0);
    oldpop[j].x2 =
        decode(oldpop[j].chrom, 1 * VAR_LENGTH, VAR_LENGTH, -20.0, 20.0);
    oldpop[j].x3 =
        decode(oldpop[j].chrom, 2 * VAR_LENGTH, VAR_LENGTH, -20.0, 20.0);
    oldpop[j].x4 =
        decode(oldpop[j].chrom, 3 * VAR_LENGTH, VAR_LENGTH, -20.0, 20.0);

    // Calcular fitness
    objfunc(&oldpop[j]);

    // Inicializar metadatos
    oldpop[j].parent[0] = 0;
    oldpop[j].parent[1] = 0;
    oldpop[j].xsite1 = 0;
    oldpop[j].xsite2 = 0;
    oldpop[j].placemut = 0;
    oldpop[j].mutation = 0;
  }
}

/* Print the initial report of the parameters given by the user */

void initial_report() {
  printf("\n      Parameters used with the Genetic Algorithm \n");
  printf(" Total population size          =       %d\n", popsize);
  printf(" Chromosome length              =       %d\n", codesize);
  printf(" Maximum number of generations  =       %d\n", Gmax);
  printf(" Crossover probability          =       %f\n", Pc);
  printf(" Mutation probability           =       %f\n", Pm);
  printf("\n\n");
}

/* Perform the memory allocation needed for the strings that will be generated
 */

void memory_allocation() {
  unsigned numbytes;
  // char *malloc();
  int i;

  /* Allocate memory for old and new populations of individuals */

  numbytes = popsize * sizeof(struct individual);

  if ((oldpop = (struct individual *)malloc(numbytes)) == NULL)
    nomemory("old population");

  if ((newpop = (struct individual *)malloc(numbytes)) == NULL)
    nomemory("new population");

  /* Allocate memory for chromosome strings in populations */

  for (i = 0; i < popsize; i++) {

    if ((oldpop[i].chrom = (unsigned *)malloc(numbytes)) == NULL)
      nomemory("old population chromosomes");

    if ((newpop[i].chrom = (unsigned *)malloc(numbytes)) == NULL)
      nomemory("new population chromosomes");

    if ((bestfit.chrom = (unsigned *)malloc(numbytes)) == NULL)
      nomemory("bestfit chromosomes");
  }

  memory_for_selection();
}

/* Allocate memory for performing tournament selection */

void memory_for_selection() {
  unsigned numbytes;
  // char *malloc();

  numbytes = popsize * sizeof(int);
  if ((tournlist = (int *)malloc(numbytes)) == NULL)
    nomemory("tournament list");

  tournsize = 2; /* Use binary tournament selection */
}

/* When done, free all memory */

void free_all() {
  if (oldpop == NULL && newpop == NULL)
    return; // Evitar liberar si ya está liberado

  for (int i = 0; i < popsize; i++) {
    if (oldpop[i].chrom != NULL) {
      free(oldpop[i].chrom);
      oldpop[i].chrom = NULL; // Marcar como liberado
    }
    if (newpop[i].chrom != NULL) {
      free(newpop[i].chrom);
      newpop[i].chrom = NULL;
    }
  }

  if (oldpop != NULL) {
    free(oldpop);
    oldpop = NULL;
  }
  if (newpop != NULL) {
    free(newpop);
    newpop = NULL;
  }
}

/* Notify if we run out of memory when generating a chromosome */

void nomemory(char *string) {
  printf("ERROR!! --> malloc: out of memory making %s\n", string);
  exit(1);
}

/* Calculate population statistics */

void statistics(struct individual *pop) {
  sumfitness = 0.0;
  min = max = pop[0].fitness;

  for (int j = 0; j < popsize; j++) {
    sumfitness += pop[j].fitness;
    if (pop[j].fitness > max) {
      max = pop[j].fitness;
      localmax = pop[j].chrom;
    }
    if (pop[j].fitness < min)
      min = pop[j].fitness;

    // ACTUALIZAR EL MEJOR FITNESS GLOBAL
    if (pop[j].fitness > bestfit.fitness) {
      memcpy(bestfit.chrom, pop[j].chrom, codesize * sizeof(unsigned));
      bestfit.x1 = pop[j].x1; // Copiar valores reales
      bestfit.x2 = pop[j].x2;
      bestfit.x3 = pop[j].x3;
      bestfit.x4 = pop[j].x4;
      bestfit.fitness = pop[j].fitness;
      bestfit.generation = gen;
    }
  }
  avg = sumfitness / popsize;

  exportar_convergencia("convergencia.txt", max, gen);
}

/* Create next batch of 55 random numbers */
void advance_random()

{
  int j1;
  double new_random;

  for (j1 = 0; j1 < 24; j1++) {
    new_random = oldrand[j1] - oldrand[j1 + 31];
    if (new_random < 0.0)
      new_random = new_random + 1.0;
    oldrand[j1] = new_random;
  }

  for (j1 = 24; j1 < 55; j1++) {
    new_random = oldrand[j1] - oldrand[j1 - 24];
    if (new_random < 0.0)
      new_random = new_random + 1.0;
    oldrand[j1] = new_random;
  }
}

/* Flip a biased coin - true if heads */

int flip(float prob)

{
  float randomperc();

  if (randomperc() <= prob)
    return (1);
  else
    return (0);
}

/* initialization routine for randomnormaldeviate */

void initrandomnormaldeviate()

{
  rndcalcflag = 1;
}

/* normal noise with specified mean & std dev: mu & sigma */

double noise(double mu, double sigma)

{
  double randomnormaldeviate();

  return ((randomnormaldeviate() * sigma) + mu);
}

/* Initialize random numbers batch */

void randomize()

{
  int j1;

  for (j1 = 0; j1 <= 54; j1++)
    oldrand[j1] = 0.0;

  jrand = 0;

  warmup_random(Rseed);
}

/* random normal deviate after ACM algorithm 267 / Box-Muller Method */

double randomnormaldeviate()

{
  // double sqrt(), log(), sin(), cos();
  float randomperc();
  double t, rndx1;

  if (rndcalcflag) {
    rndx1 = sqrt(-2.0 * log((double)randomperc()));
    t = 6.2831853072 * (double)randomperc();
    rndx2 = sin(t);
    rndcalcflag = 0;
    return (rndx1 * cos(t));
  } else {
    rndcalcflag = 1;
    return (rndx2);
  }
}

/* Fetch a single random number between 0.0 and 1.0 - Subtractive Method */
/* See Knuth, D. (1969), v. 2 for details */
/* name changed from random() to avoid library conflicts on some machines*/

float randomperc()

{
  jrand++;

  if (jrand >= 55) {
    jrand = 1;
    advance_random();
  }

  return ((float)oldrand[jrand]);
}

/* Pick a random integer between low and high */

int rnd(int low, int high)

{
  int i;
  float randomperc();

  if (low >= high)
    i = low;

  else

  {
    i = (randomperc() * (high - low + 1)) + low;
    if (i > high)
      i = high;
  }

  return (i);
}

/* real random number between specified limits */

float rndreal(float lo, float hi)

{
  return ((randomperc() * (hi - lo)) + lo);
}

/* Get random off and running */

void warmup_random(float random_seed)

{
  int j1, ii;
  double new_random, prev_random;

  oldrand[54] = random_seed;
  new_random = 0.000000001;
  prev_random = random_seed;

  for (j1 = 1; j1 <= 54; j1++)

  {
    ii = (21 * j1) % 54;
    oldrand[ii] = new_random;
    new_random = prev_random - new_random;
    if (new_random < 0.0)
      new_random = new_random + 1.0;
    prev_random = oldrand[ii];
  }

  advance_random();
  advance_random();
  advance_random();

  jrand = 0;
}

/* Perform all the work involved with the creation of a new generation of
 * chromosomes */

void generation() {

  int mate1, mate2, jcross1, j = 0;

  /* perform any preselection actions necessary before generation */

  // preselect();
  double avg_fitness[popsize];

  double sumfit = 0.0;

  // RULETA
  sumfit = f_med(oldpop, avg_fitness);
  /* select, crossover, and mutation */

  // APLICAR ELITISMO
  elitismo(oldpop, newpop);
  j = 1;
  do {
    /* pick a pair of mates */

    mate1 = ruleta(sumfit, avg_fitness);
    mate2 = ruleta(sumfit, avg_fitness);

    /* Crossover and mutation */

    crossover(oldpop[mate1].chrom, oldpop[mate2].chrom, newpop[j].chrom,
              newpop[j + 1].chrom, &jcross1);

    mutation(&(newpop[j]));
    mutation(&(newpop[j + 1]));

    /* Decode string, evaluate fitness, & record */
    /* parentage date on both children */

    decode_all_vars(&newpop[j], -20.0, 20.0);
    decode_all_vars(&newpop[j + 1], -20.0, 20.0);

    // newpop[j].x = decode(newpop[j].chrom, codesize);
    // newpop[j + 1].x = decode(newpop[j + 1].chrom, codesize);

    objfunc(&(newpop[j]));
    newpop[j].parent[0] = mate1 + 1;
    newpop[j].xsite1 = jcross1;
    newpop[j].xsite2 = 0;
    newpop[j].parent[1] = mate2 + 1;
    objfunc(&(newpop[j + 1]));
    newpop[j + 1].parent[0] = mate1 + 1;
    newpop[j + 1].xsite1 = jcross1;
    newpop[j + 1].xsite2 = 0;
    newpop[j + 1].parent[1] = mate2 + 1;

    /* Increment population index */

    j = j + 2;
  }

  while (j < (popsize - 2));
}

// ELITISMO
void elitismo(struct individual *oldpop, struct individual *newpop) {
  int best_ind = 0;
  double best_fit = oldpop[0].fitness;

  // ENCONTRAR AL MEJOR INVIDIDUO
  for (int i = 0; i < popsize; i++) {
    if (oldpop[i].fitness > best_fit) {
      best_ind = i;
      best_fit = oldpop[i].fitness;
    }
  }

  // COPIAR EL MEJOR INVIDIDUO
  memcpy(&newpop[0], &oldpop[best_ind], sizeof(struct individual));
  newpop[0].parent[0] = -1;
  newpop[0].parent[1] = -1;
}

// FUNCION PARA CALCULAR LA MEDIA DEL FITNESS DE LA POBLACION
double f_med(struct individual *ind, double *avg_fit) {
  double med = 0.0;
  double sum_fitness = 0.0;

  for (int i = 0; i < popsize; i++)
    avg_fit[i] = 0.0;

  for (int i = 0; i < popsize; i++) {
    sum_fitness += ind[i].fitness;
  }
  med = sum_fitness / (double)popsize;

  for (int i = 0; i < popsize; i++) {
    avg_fit[i] += ind[i].fitness / med;
  }

  return sum_fitness;
}

// FUNCION DE SELECCION POR RULETA
int ruleta(double sumfitness, double *avg_fit) {
  float r = rndreal(0.0, popsize);
  double accum = 0.0;
  int i = 0;
  do {
    accum += avg_fit[i];
    i++;
  } while (accum < r && i < popsize);

  return i - 1;
}

/* Perform a mutation in a random string, and keep track of it */
// FUNCION DE MUTACION MODIFICADA PARA INTERCAMBIAR EL BIT A LO LARGO DE LA
// CADENA SI SE CUMPLE UNA PROBABILIDAD (Pm)
void mutation(struct individual *ind) {
  // MUTACION POR BIT EN TODA LA CADENA

  for (int i = 0; i < codesize; i++) {
    if (flip(Pm)) {
      if (ind->chrom[i] == 0) {
        ind->chrom[i] = 1;
      } else {
        ind->chrom[i] = 0;
      }
      nmutation++;
    }
  }
}

/* Cross 2 parent strings, place in 2 child strings */
// FUNCION DE CRUZA MODIFICADA PARA HACER LA CRUZA DE UN PUNTO
void crossover(unsigned *parent1, unsigned *parent2, unsigned *child1,
               unsigned *child2, int *jcross) {

  int j = 0;
  if (flip(Pc)) {
    *jcross = rnd(1, codesize - 1);
    ncross++;

    // PARTE 1 DEL PADRE_1 AL HIJO_1 Y PARTE 1 DEL PADRE_2 AL HIJO_2
    for (j = 0; j < *jcross; j++) {
      child1[j] = parent1[j];
      child2[j] = parent2[j];
    }

    // PARTE 2 DEL PADRE_2 AL HIJO_1 Y PARTE 2 DEL PADRE_1 AL HIJO_2
    for (j = *jcross; j < codesize; j++) {
      child1[j] = parent2[j];
      child2[j] = parent1[j];
    }
  } else {
    // SE COPIAN LOS LOS VALORES DE LOS PADRES A LOS HIJOS SIN MODIFICACIONES
    for (j = 0; j < codesize; j++) {
      child1[j] = parent1[j];
      child2[j] = parent2[j];
    }
    *jcross = 0;
  }
}

/* Write the population report on the screen */

void report() {
  double fx_local = 1.0 / max - 1.0;
  double fx_global = 1.0 / bestfit.fitness - 1.0;

  printf("\n--- Generación %d ---\n"
         "Estadísticas:  Media:        Máxima:       Mínima:       Cruzas:     "
         "Bits mutados:\n"
         "               %.6f    %.6f    %.6f      %d         %d\n",

         gen, avg, max, min, ncross, nmutation);

  printf("\nMejor solución (esta generación):\n");
  printf("Cadena binaria: ");
  writechrom(localmax);
  printf("\n  Valores: x1=%.6f x2=%.6f x3=%.6f x4=%.6f | Fitness: %.6f | "
         "f(x)=%.6f\n",
         bestfit.x1, bestfit.x2, bestfit.x3, bestfit.x4, max, fx_local);
}

/* Print out each chromosome from the most significant bit to the least
 * significant bit */

void writechrom(unsigned *chrom) {
  int j;

  for (j = (codesize - 1); j >= 0; j--) {
    if (chrom[j] == 0)
      printf("0");
    else
      printf("1");
  }
}

/* Use a system call to clear the screen */

void cls()

{
  system("clear");
}

/* Decode the chromosome string into its corresponding decimal value */
double decode(unsigned *chrom, int start, int length, double min, double max) {
  double accum = 0.0;
  for (int j = 0; j < length; j++) {
    if (chrom[start + j] == 1) {
      accum += pow(2.0, (double)j);
    }
  }

  // AJUSTE DE LOS VALORES AL RANGO [-20, 20] USANDO LA ECUACION DE LA RECTA
  double val = min + (max - min) * (accum / (pow(2.0, (double)length) - 1.0));
  return val;
}

// FUNCION AUXILIAR PARA DECODIFICAR LAS 4 VARIABLES
void decode_all_vars(struct individual *ind, double min, double max) {
  ind->x1 = decode(ind->chrom, 0 * VAR_LENGTH, VAR_LENGTH, min, max);
  ind->x2 = decode(ind->chrom, 1 * VAR_LENGTH, VAR_LENGTH, min, max);
  ind->x3 = decode(ind->chrom, 2 * VAR_LENGTH, VAR_LENGTH, min, max);
  ind->x4 = decode(ind->chrom, 3 * VAR_LENGTH, VAR_LENGTH, min, max);
}

/* Define the objective function. In this case, f(x) = x^2 */
// FUNCION OBJETIVO ->
void objfunc(struct individual *ind) {

  double term1 = pow(10.0 * (ind->x2 - ind->x1 * ind->x1), 2);
  double term2 = pow(1.0 - ind->x1, 2);
  double term3 = 90.0 * pow(ind->x4 - ind->x3 * ind->x3, 2);
  double term4 = pow(1.0 - ind->x3, 2);
  double term5 = 10.0 * pow(ind->x2 + ind->x4 - 2.0, 2);
  double term6 = 0.1 * pow(ind->x2 - ind->x4, 2);
  ind->fitness = 0.0;

  // MINIMIZAR
  ind->fitness = 1.0 / (1.0 + term1 + term2 + term3 + term4 + term5 + term6);
}
