#include <iostream>
#include <vector> 
#include "citiesReader.h"
#include <stdlib.h>
#include <stdio.h>
#include "mst.hpp"
#include <chrono> 

using namespace std;
using namespace std::chrono;

void saveSolution( vector<Edge>& solution){
  FILE* fileOut = NULL;
  fileOut = fopen("resuGraph.dat", "w");
  for(int i=0; i< solution.size() ; i++){
      fprintf(fileOut, "%d %d\n", solution[i].from, solution[i].to);
  }
  fclose(fileOut);
}


int main(int argc, char **argv)
{

    if (argc != 2)
    {
        perror("Veuillez passer une population minimale en paramètre du programme");
        return EXIT_FAILURE;
    }
    //-----------------------------------------------------------------
    //--- READING cities
    //-----------------------------------------------------------------

    int popMin = atoi(argv[1]);

    ListOfCities *cities;
    cities = citiesReader(popMin);
    printf("== cities have been read ==");

    auto start = high_resolution_clock::now(); 
    Mst mst(cities); 
    vector<Edge> solution =  mst.solve();
    auto stop = high_resolution_clock::now(); 

    auto duration = duration_cast<microseconds>(stop - start); 

    cout << "For (" << cities->number << ") cities " << endl; 
    cout << "Executuin time: " << duration.count() << " microseconds" << endl;


    cout << cities->number << endl ; 
    cout << solution.size() << endl ; 


    saveSolution(solution); 

    
    freeListOfCities(cities); 
    return 0;
}
