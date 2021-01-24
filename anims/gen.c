/*

                     Animation of a Genetic Algorithm
                                   by
                              David Brogan



        This program attempts to model the solving of the traveling salesman
problem through the use of a genetic algorithm.  A genetic algorithm
presents a set of potential solutions, and weights them.  The solutions
believed to be the best will continue in the pursuit of reaching a better
state, whereas the poorest solutions will be tossed aside.  There are
some solutions in the middle of the two extremes that can be of a special
use.  It is believed that these solutions contain a partial solution, and
the genetic algorithm seeks to randomly combine these average solutions in
the hopes that a much improved one will result.

        For my project, I had to remove much of the efficiency of genetic
algorithms for the purpose of demonstrating how they work.  At any
given time, my system compares three solutions.  The best of these will
always go on to the next round.  The three then have equal opportunities
of contributing to the next generation's solutions.  This occurs by
crossovers.  These occur when a continuous portion of one solution is
combined with whatever can be scrapped together from another solution.

        The layout of my xtango screen is split into halves.  The top
half contains the "map" of cities that must be joined by the smallest
possible route.  At the end of each generation, the best solution in the
set will be represented on the "map" with lines going from node to node.
The lower half of my screen has a left portion and a right portion.  On the
left, the current generation of solutions is drawn.  The best of these will
move to the right to form the first of the new generation.  After that
occurs, a randomly sized portion of a solution is moved down to the bottom
of the screen to form the left most sequence of a new solution.  Additional
nodes are added to this new solution from a randomly chosen old solution.
The color coding is hoped to demonstrate the formation of new solutions.
To increase the likelihood that something productive might come out of
this, mutations occur.  These help to keep the solutions from remaining
too close to the paths of their parent's.  After a whole new set of
solutions has been generated, and are situated on the right, the whole
set of old solutions is erased, and the new ones are re-drawn on the left
in unique colors.  The path on the map is redrawn, and the process repeats.

*/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include "xtango.h"

#define MAX_LENGTH  10          /* Maximum Number of Cities in Tour */
#define POPULATION_SIZE 3      /* Maximum Size of GA population    */
#define SQR(A)  ((A)*(A))
#define MUTATION_RATE 0.85   /* a mutation is likely to occur only 1 out of 100 times */
#define generationsToSkip 1

void ANIMInitCities ();
void ANIMDrawPaths ();
void ANIMRemovePaths ();
void ANIMShowGenes ();
void ANIMMoveResults ();
void ANIMRemoveGene ();
void ANIMMoveBest ();
void ANIMRemoveSolution ();
void ANIMRemoveResults ();
void ANIMDropDown ();
void ANIMMutate ();

static NAME_FUNCT fcn[] = {{"InitCities",  1, {VOID, (FPTR)ANIMInitCities}},
                           {"DrawPaths", 1, {VOID, (FPTR) ANIMDrawPaths}},
	  	           {"RemovePaths", 1, {VOID, (FPTR) ANIMRemovePaths}},
                           {"RemoveGene", 1, {VOID, (FPTR) ANIMRemoveGene}},
 	                   {"MoveResults", 1, {VOID, (FPTR) ANIMMoveResults}},
                           {"RemoveSolution", 1, {VOID, (FPTR) ANIMRemoveSolution}},
   	                   {"RemoveResults", 1, {VOID, (FPTR) ANIMRemoveResults}},
                           {"ShowGenes", 1, {VOID, (FPTR) ANIMShowGenes}},
                           {"DropDown", 1, {VOID, (FPTR) ANIMDropDown}},
                           {"MoveBest", 1, {VOID, (FPTR) ANIMMoveBest}},
                           {"Mutate", 1, {VOID, (FPTR) ANIMMutate}},
                           {NULL, NULL, NULL, NULL}};

int cities[] = {200, 342, 139, 693, 367, 842, 10, 506, 446, 748, 360, 24, 123, 400, 240, 72, 234, 225, 455, 320};



typedef struct chromstruct
    {
      int chrom[MAX_LENGTH];
      long       tourLength;
      double      relativeFitness;
    } population[POPULATION_SIZE];


/*
typedef struct pnt {
                int x;
                int y;
        };
*/

int generation = 0;
int path[MAX_LENGTH]; /* slimy: use 'point', with x=first city, y=second city */
population newpopulation, oldpopulation;


/*
// displayChromosome
// ------------------
//
// prints out a chromosome
//
// INPUTS: chromsome to be printed, length of chromosome
//
*/

void displayChromosome (chrom, length)
int chrom[MAX_LENGTH];
int length;
 {
  int count;

  for (count =0 ; count < length; count ++)
   {
    printf ("%d.",chrom[count]);
   }
  printf ("\n");
 }




/*
// getRandom
// ------------------
//
// this gets a random integer between 0 and the input (i)-1.
//
// INPUTS: an integer for the range.
// OUTPUT: random integer.
*/

int getRandom (i)
int i;
 {
  return ( (random () % i) );
 }

/*
// copyPopulation
// --------------------
//
// Inputs: two populations, TO,FROM
// Ouputs: TO = FROM
//
*/

void copyPopulation (TOPOP, FROMPOP)
population TOPOP;
population FROMPOP;
 {
  int i;

  for (i = 0 ; i < POPULATION_SIZE; ++i)
     TOPOP [i] = FROMPOP [i];
 }


/*
// copyChromosome
// --------------------
//
// Inputs: two chromosomes, TO,FROM
// Ouputs: TO = FROM
//
*/

void copyChromosome (TOCHROM, FROMCHROM, length)
int TOCHROM[MAX_LENGTH];
int FROMCHROM[MAX_LENGTH];
int length;
 {
  int i;

  for (i = 0 ; i < length; ++i) {
     TOCHROM [i] = FROMCHROM [i];
  }
  }



 
int	numberOfCities()
{
return(MAX_LENGTH);
}


int	cityDistance(i, j)	
int i;
int j;	

{

    int temp;
    double tempd;
    double x1,x2,y1,y2;


    x1 = cities[i];
    x2 = cities[j];
    y1 = cities[i+MAX_LENGTH];
    y2 = cities[j+MAX_LENGTH];
 

    tempd = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) ) ;
    temp  = (int) tempd;

    return(temp);


}
 
/*
void addToPath(i, j) 
int i;
int j;
{

    struct pnt p;

    p.x = i;
    p.y = j;

} 

*/

/*
// findTourLength
// ------------------------------
//
// this finds the Tour-length (integer) for a particular chromosome.
*/

long findTourLength (path, numberofcities)
int path[MAX_LENGTH];
int numberofcities;

{
  int count;
  long total =0;
  
  for (count = 0; count < numberofcities -1 ; count ++)
   {
    total  += cityDistance (path[count], path [count +1]);
   }
  total += cityDistance (path [numberofcities - 1], path[0]);
  printf ("the total is: %ld\n", total); 
return (total);

}



/*
// findRelativeFitness     CONSIDER CHANGING THIS ALSO !!!!
// --------------------------------------------------------------
//
// this procedure should find the relative fitness of each 
// chromosome.
//
// remember that the smaller the path length, the higher
//   the relative fitness
//
// 
*/

void findRelativeFitness (toBeChecked)
population toBeChecked;
 
{
  int count;
  double normalization = 0.0,temp;



  for (count = 0; count < POPULATION_SIZE; count ++)
    {
      temp = (double) toBeChecked[count].tourLength;
      toBeChecked [count].relativeFitness = (double) (1.0 / temp);
      normalization += toBeChecked[count].relativeFitness;
    }

/* Normalize */
  
  for (count = 0; count < POPULATION_SIZE; count ++)
     {
      toBeChecked[count].relativeFitness = toBeChecked[count].relativeFitness/normalization;
     }


}


/* 
// mutate 
// ------------------
// 
// this randomly mutates a chromosome
//
// INPUTS: the chromosome to be mutated.
// OUTPUT: the chromosome is modified.
*/


void mutate (chrom, probability, length,cond)
int chrom[MAX_LENGTH];
double probability;
int length;
int cond;

  {
    int switch1,switch2,store;
    double prob;
    
    prob = (double) ((double) getRandom (10000)) / ((double)10000.0);    
    
    if (prob <= probability)                /*mutations only happen with a */
     {					    /*certain probability. */
          switch1 = getRandom (length);
          switch2 = getRandom (length);

          while (switch1 == switch2)  {
            switch1 = getRandom (length);
            switch2 = getRandom (length);
          }
          store = chrom [switch1];
          chrom[switch1] = chrom [switch2];
          chrom[switch2] = store;
          if (cond == 1)
          TANGOalgoOp (fcn, "Mutate", switch1, switch2);
     }	
  }
  



/* 
// initializeChromosome 
// --------------------
// 
// this routine just creates a chromosome with the cities ranging from 
// 0 to n. The cities are ordered randomly.
//
// INPUTS : chromosome to be modified and the number of cities.
// OUTPUT : the chromosome is modified.
*/


void initializeChromosome (chrom, length)
int chrom[MAX_LENGTH];
int length;

{
  int count,mutateTimes;
 
  for (count = 0; count < length; count ++)
    chrom [count] = count;


  /* to make this random, mutate it ---> a lot! 
     between 100 & 200 times */
    
  mutateTimes = getRandom (100) + 100;

  for (count = 0; count < mutateTimes; count ++)
     {
     mutate (chrom, 1.0, length, 0);
     }


}




/*
// initializePopulation
// ----------------------------
//
// this creates the first population.
*/

void initializePopulation (newpop, length)
population newpop;
int length;
  {
   int i,total;
  

   for (i = 0; i < POPULATION_SIZE; i++)  
    {
      initializeChromosome (newpop[i].chrom,length);
      newpop[i].tourLength = findTourLength (newpop[i].chrom,length);
    }

    findRelativeFitness (newpop);
  }




/*
// crossover             
// -------------------------------------------------------
//
// Crossover operators combine two chromosome to produce the
//    children. 
// Normal Crossover operators such as the 2 pnt crossover cannot
//    be used.
// Your job is to develop crossovers which can be used by the GA
//    to produce reasonable answers to the TSP problem.
//
// The crossover installed here randomly chooses one chromosome
//    and mutates it.  Your crossover routines should:
//        ..somehow - at least minimally combine the information
//                     between the two chromosomes.
*/



void crossover (t1, t2, one, two, result, length)
int t1;
int t2;
int one[MAX_LENGTH];
int two[MAX_LENGTH];
int result[MAX_LENGTH];
int length; 

{
  int pick;
  int i, j, k, l, temp, check;
  int tempResult[MAX_LENGTH];
  int x1, x2;
 
  x1 = getRandom (length);
  x2 = getRandom (length);

  for (i = 0; i < length; ++i)
    tempResult[i] = -1;
  if (x1 > x2)
    {
      temp = x1;
      x1 = x2;
      x2 = temp;
    }
  temp = 0;
  for (l = x1; l <= x2; ++l)
    {
    tempResult[temp] = one[l];
    TANGOalgoOp (fcn, "DropDown", t1, l, temp);
    temp = temp + 1;
    }
  
  for (l = 0; l < length; ++l)
    {
    check = 1;
    for (k = 0; k <= (x2 - x1 + 1); k++)
      if ((tempResult[k] != two[l]) && (check == 1))
        check = 1;
      else check =0;
    if (check == 1)
      {
      tempResult[temp] = two[l];
      TANGOalgoOp (fcn, "DropDown", t2, l, temp);
      temp = temp + 1;
      }
     }

  copyChromosome (result, tempResult, length);
  mutate (tempResult,MUTATION_RATE,length,1);
  
}

/*
//  selectFromPopulation   THIS MUST BE MODIFIED !!
//  --------------------------------------------------
//
//  this routine just selects the best chromosome.
//  however, what is really wanted is that the chromosomes
//  are chosen by their relative fitness -- through probabilities,
//  that means if 
//
//   chromosome A had 5 
//              B     10
                C     15            

//   A would be selected 17% of the time
//   B would be selected 33% of the time
//   C would be selected 50% of the time
//
//   this routine should use probabilities, not an algorithm to 
//   automaticlly select the best one.
*/



int selectFromPopulation (from)
population from;

 {
   double prob;
   double fitMax;
   int tempMax;
   int i;

   prob = (double) ((double) getRandom (10000)) / ((double) 10000.0);
   fitMax = 0.0;
   tempMax = -1;
   for (i = 0; i < POPULATION_SIZE; ++i)
     if ((prob > from[i].relativeFitness ) && (from[i].relativeFitness >= fitMax))
	{
	tempMax = i;
	fitMax = from[i].relativeFitness;
	}
   if (tempMax == -1)
        for (i = 0; i < POPULATION_SIZE; ++i)
        if ((prob < from[i].relativeFitness ) && (from[i].relativeFitness >= fitMax))
          {
           tempMax = i;
           fitMax = from[i].relativeFitness;
          }

 
   return (getRandom(3));

 }


/*
//  nextGeneration
//  --------------------------
// 
//  this calculates the next generation.
//
*/



void nextGeneration (oldpopulation , newpopulation, length, generation)
population oldpopulation;
population newpopulation;
int length;
int generation;
  {
   int temp1[MAX_LENGTH], temp2[MAX_LENGTH];
   int count, t1,t2, place;
   int temp[10];	
int i;
long min;

   printf ("Welcome to nextGeneration\n");
   min = oldpopulation[0].tourLength;

   for (count = 0; count < POPULATION_SIZE; count ++)
   {
     copyChromosome (temp, oldpopulation[count].chrom,10);
     TANGOalgoOp (fcn, "ShowGenes", temp, count);
   }

   for (count = 0; count < POPULATION_SIZE; ++count)
     {
	if (oldpopulation[count].tourLength <= min)
	  {
	    min = oldpopulation[count].tourLength;
            place = count;
 	  }
     }
   printf ("We now have found the best one\n");

  
   copyChromosome (newpopulation[0].chrom, oldpopulation[place].chrom, length); 

   TANGOalgoOp (fcn, "MoveBest", place);

   newpopulation[0].tourLength = oldpopulation[place].tourLength;
   printf ("The best one is now part of the newpopulation generation\n");

   for (count = 1; count < POPULATION_SIZE; ++count)
    {
      t1 = selectFromPopulation (oldpopulation);
      copyChromosome (temp1, oldpopulation[t1].chrom,length);

      t2 = selectFromPopulation (oldpopulation);
      copyChromosome (temp2, oldpopulation[t2].chrom,length);
      crossover (t1, t2, temp1,temp2,newpopulation[count].chrom,length);
      mutate (newpopulation[count].chrom,MUTATION_RATE, length,1);
      TANGOalgoOp (fcn, "MoveResults", count);
      TANGOalgoOp (fcn, "RemoveResults");
      newpopulation[count].tourLength = findTourLength (newpopulation[count].chrom, length);
    }
   for (count = 0; count < POPULATION_SIZE; count ++)
   {
     copyChromosome (temp, oldpopulation[count].chrom,10);
     TANGOalgoOp (fcn, "RemoveGene", temp, count);
   }
   for (count = 0; count < POPULATION_SIZE; count ++)
   {
     TANGOalgoOp (fcn, "RemoveSolution", count);
   }
   if (generation > 1)
   TANGOalgoOp (fcn, "RemovePaths");


   printf ("We are now ready to check the fitness\n");
   findRelativeFitness (newpopulation);
   generation ++;
   printf ("and goodbye\n");
  }


/*
// next
// --------------------
//
// this is the SUIT interface to the GA program.
// 
// it is basically a call to nextGeneration and the initial set-up.
//
//
*/  


void next ()
 {

  int count, skip,keep,cnt;
  long mintour;
  int send_this[MAX_LENGTH];
  time_t t;

  srandom ((unsigned) time(&t)); 
  while (generation <= 500) {

  if (generation == 0) 
       {
          initializePopulation (oldpopulation,numberOfCities ());
       }

  skip = generationsToSkip + generation;

  for (count = generation; count < skip; count ++)
    { 
      nextGeneration (oldpopulation, newpopulation,numberOfCities (),generation);
      copyPopulation (oldpopulation, newpopulation); 
    }
  generation = count;
  /** select the best chromosome to display **/
  
/*  TANGOalgoOp (fcn, "ShowGenes", oldpopulation); */

  mintour = oldpopulation [0].tourLength;   
  keep = 0;

  for (count = 0; count < POPULATION_SIZE; ++count)
    {
      if (oldpopulation[count].tourLength <= mintour)
        {
          mintour = oldpopulation [count].tourLength;
          keep = count;
        }
    }
  printf ("The length is :%ld  ",mintour);
  copyChromosome (send_this, oldpopulation[keep].chrom,MAX_LENGTH); 
  displayChromosome (oldpopulation[keep].chrom,MAX_LENGTH); 
  TANGOalgoOp (fcn, "DrawPaths", send_this);  
 } 
}
    

main ()
{
  int x;

  printf("Genetic algorithms\n\n");
  TANGOalgoOp (fcn,"BEGIN"); 
  TANGOalgoOp (fcn,"InitCities",cities);  

  next (); 
  TANGOalgoOp (fcn,"END"); 
}
