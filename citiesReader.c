#include "citiesReader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
   Create a data structure ListOfCities and allocates the necessary space to store
   a given number of cities.
   @param number is the number of cities the data structure will store.
   @return a reference to a ListOfCities.
   @requires number > 0
 */
ListOfCities* createCities(int number){
    ListOfCities* cities = (ListOfCities* )malloc(sizeof(ListOfCities));
    int i;
    
    cities->number = number;
    // Allocate arrays
    cities->name = (char ** ) malloc(cities->number*sizeof(char*));
    for(i = 0; i < number; i++)
      cities->name[i] = (char*) malloc(32*sizeof(char));
    cities->pop  = (int*) malloc(cities->number*sizeof(int));
    cities->lon  = (float *)malloc(cities->number*sizeof(float));
    cities->lat  = (float *)malloc(cities->number*sizeof(float));

    return cities;
}

/**
   Liberate space occupied by a ListOfCities
   @param cities is a ListOfCities.
   @requires cities != NULL
*/
void freeListOfCities(ListOfCities * cities){
  free(cities->name);
  free(cities->pop);
  free(cities->lon);
  free(cities->lat);
  free(cities);
}



/**
   Count the number of cities with population greater than or equal to 'popMin'
   @param inputFile is a CSV file containing a list of cities with their
   population and coordinates.
   @param popMin is a threshold above which a city will be counted
   @return number of cities with a population above popMin
   @requires inputFile != NULL 
   @requires popMin > 0
*/
int countCitiesWithMinPopulation(FILE* inputFile, int popMin){
  int number = 0, myPop , i;
  char line[512];
  const char s[2] = ",";
  char *token;
  
  while(fgets(line, 512, inputFile) != NULL){
    /* Select the 14th coma separated value and stores it in 'token' */
    token = strtok(line, s);
    for(i = 0; i < 14; i++) token = strtok(NULL, s);

    myPop = atoi(token);
    if(myPop >= popMin) number++;
  }
  /* Rewind file */
  fseek(inputFile, 0, SEEK_SET); 
  
  return number;
}

/**
   Load cities population and coordinates from file to a ListOfCities data structure.
   @param inputFile is the file from which data are loaded.
   @param popMin is the minimum polution a city has to have in order to be loaded.
   @param cities is a ListOfCities data structure in which data will be loaded.
   @requires inputFile != NULL
   @requires popMin > 0
   @requires cities != NULL and cities->number = countCitiesWithMinPopulation(inputFile, popMin)
 */
void loadListOfCities(FILE* inputFile, int popMin, ListOfCities* cities){
  // Reading/Saving data
  int index=0, i, myPop;
  char line[512];
  const char s[2] = ",";
  char *token;
  char myName[32];
  
  while(fgets(line, 512, inputFile) != NULL){
      
    token = strtok(line, s);
    for(i=0; i<3;  i++) token = strtok(NULL, s);
    strncpy(myName, token, 32);
    for(i=0; i<11; i++) token = strtok(NULL, s);
    myPop = atoi(token);

    if(myPop >= popMin){
      for(i=0; i<5;  i++) token = strtok(NULL, s);
      cities->lon[index] = atof(token);
      for(i=0; i<1;  i++) token = strtok(NULL, s);
      cities->lat[index] = atof(token);
    
      strncpy(cities->name[index], myName, 32);
      cities->pop[index] = myPop;
      index++;
    }
  }

}

/**
   Writes content of a ListOfCities data structure in the 'resuCities.dat' file.
   @param cities is the ListOfCities data structure to save of disk.
   @requires cities != NULL
*/
void saveListOfCities(ListOfCities* cities){
  FILE* outputFile = NULL;
  if( (outputFile = fopen("resuCities.dat", "w")) == NULL){
    perror("Could not open file resuCities.dat");
    exit(-1);
  }
  
  for(int i=0; i<cities->number; i++)
    fprintf(outputFile, "%i %f %f\n", cities->pop[i], cities->lon[i], cities->lat[i]);
  fclose(outputFile);
}


/**
   Loads the name, population and coordinates of all the cities above 
   a given population threshold, store the result in a file named 'resuCities.dat'
   and return a ListOfCities data structure containing the list.
   @param popMin is the population threshold for a city to be considered.
   @return a reference to a ListOfCities containing the name, population and coordinates of all the    cities whose population is above 'popMin'.
   @requires popMin > 0
   @requires 'citiesList.csv' file to exist on disk and to be readable by the program.
*/
ListOfCities* citiesReader(int popMin){
  /* READING cities with population greater than or equal to 'popMin' */
  printf("== Reading cities with population >= %i from 'citiesList.csv' ==\n", popMin);

  FILE* inputFile = NULL;
  if( (inputFile = fopen("citiesList.csv", "r")) == NULL){
    perror("Could not open file citiesList.csv");
    exit(-1);
  }

  ListOfCities* cities = createCities(countCitiesWithMinPopulation(inputFile, popMin));
  loadListOfCities(inputFile, popMin, cities);
 
  fclose(inputFile);
  
  /* WRITING cities with population greater than or equal to 'popMin' */
  printf("== Writing cities with population >= %i in 'resuCities.dat' ==\n", popMin);
  saveListOfCities(cities);

  return cities;
}
