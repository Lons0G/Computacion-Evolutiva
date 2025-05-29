/* Global structures and variables */

struct individual {
  unsigned *chrom; /*      chromosome string for the individual */
  double x1;       /*	          value of the decoded string */
  double x2;       /*	          value of the decoded string */
  double x3;       /*	          value of the decoded string */
  double x4;       /*	          value of the decoded string */
  double fitness;  /* 	            fitness of the individual */
  int xsite1;      /* 	           crossover site 1 at mating */
  int xsite2;      /* 	           crossover site 2 at mating */
  int placemut;    /*			       mutation place */
  int mutation;    /*		           mutation indicator */
  int parent[2];   /*         who the parents of offspring were */
};

struct bestever {
  unsigned *chrom; /* chromosome string for the best-ever individual */
  double x1;       /*	          value of the decoded string */
  double x2;       /*	          value of the decoded string */
  double x3;       /*	          value of the decoded string */
  double x4;       /*	          value of the decoded string */
  double fitness;  /*            fitness of the best-ever individual */
  int generation;  /*                   generation which produced it */
};

/* Functions prototypes */

void memory_allocation();
void memory_for_selection();
void free_all();
void free_selection();
void nomemory(char *);
void initialize_pop();
void initial_report();
void statistics(struct individual *);
void warmup_random(float);
float rndreal(float, float);
int rnd(int, int);
float randomperc();
double randomnormaldeviate();
void randomize();
double noise(double, double);
void initrandomnormaldeviate();
int flip(float);
void advance_random();
void generation();
int select();
void reset();
void preselect();
void mutation(struct individual *);
void crossover(unsigned *, unsigned *, unsigned *, unsigned *, int *);
void report();
void writepop();
void writechrom(unsigned *);
void cls();
double decode(unsigned *, int, int, double, double);
void decode_all_vars(struct individual *, double, double);
void objfunc(struct individual *);
double f_med(struct individual *, double *);
int ruleta(double, double *);
void elitismo(struct individual *, struct individual *);

static int *tournlist, tournpos,
    tournsize;             /* Tournment list, position in list */
struct individual *oldpop; /* last generation of individuals */
struct individual *newpop; /* next generation of individuals */
struct bestever bestfit;   /* fittest individual so far */
double sumfitness;         /* summed fitness for entire population */
double max;                /* maximum fitness of population */
unsigned *localmax;        /* String corresponding to the local maximum */
double avg;                /* average fitness of population */
double min;                /* minimum fitness of population */
float Pc;                  /* probability of crossover */
float Pm;                  /* probability of mutation */
int gen;                   /* current generation number */
int Gmax;                  /* maximum number of generations */
int nmutation;             /* number of mutations */
int ncross;                /* number of crossovers */
float Rseed;               /* Random numbers seed */
double oldrand[55];        /* Array of 55 random numbers */
int jrand;                 /* current random number */
double rndx2;              /* used with random normal deviate */
int rndcalcflag;           /* used with random normal deviate */
