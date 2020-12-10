#ifndef MST
#define MST
#include <vector>
#include <queue>
#include "citiesReader.h"
#include <iostream>
#include <math.h>
using namespace std;

class Edge
{

public:
    int from, to;
    double weight;

    Edge(int from, int to, double weight)
    {
        this->from = from;
        this->to = to;
        this->weight = weight;
    }
    bool operator<(const Edge &op) const
    {
        return this->weight > op.weight;
    }
};
class Mst
{

    /**
     * @property adjlist
     * La representation de la graph 
     * le premier noeud est 0 
     * le dernier noeud est (adjlist.size - 1 )
     * adjlist[0][1]  contient le poid d'arret (0 --> 1)*/
    vector<vector<double>> adjList;

    ListOfCities *cities;
    void createGraph()
    {
        printf("== Creating the adjacent list == \n"); 
        vector<vector<double>> adjList(cities->number, vector<double>(cities->number, 1e18));

        for (int i = 0; i < cities->number; i++)
        {
            for (int j = i + 1; j < cities->number; j++)
            {
                adjList[i][j] = adjList[j][i] = this->calcDistance(i, j);
            }
        }
        this->adjList = adjList;
    }

    double calcDistance(int i, int j)
    {   

        auto toRadians = [](double degree){
            const double PI = acos(-1); 
            return PI * degree / 180 ; 
        }; 

        const double R = 6371;
        double lata = toRadians(cities->lat[i]); 
        double lona = toRadians(cities->lon[i]); 

        double latb = toRadians(cities->lat[j]); 
        double lonb = toRadians(cities->lon[j]);

        //R · acos( sin(lata) sin(latb) + cos(lona − lonb) cos(lata) cos(latb) )
        return R * acos(
                       sin(lata) * sin(latb) +
                       cos(lona - lonb) * cos(lata) * cos(latb));
        
    }

public:
    Mst(ListOfCities *cities)
    {
        this->cities = cities;
        this->createGraph();
    }
    /**
     * @return vector<Edge> la solution 
    */
    vector<Edge> solve()
    {

        printf("== Finding the MST of the graph == \n"); 
        vector<Edge> solution;
        double cost = 0;
        priority_queue<Edge> Q;                  // tas minimum pour chosir le minimum arret
        vector<bool> visited(adjList.size(), 0); // visited[i] indique si on est passé par le noeud i
        Q.push(Edge(-1, 0, 0));

        while (!Q.empty())
        {
            Edge currentEdge = Q.top(); // on prend le minimum arret
            Q.pop();                // suprrime
            int current = currentEdge.to;
            if (visited[current])
                continue; // si on est passé par le noeud on l'ignore

            visited[current] = true; // marque qu'on est passé par ce noeud
            if (currentEdge.from != -1)     // si c'est pas le premier arret
                solution.push_back(currentEdge);
            cost += currentEdge.weight ;
            for (int child = 0; child < adjList.size(); child++)
            {
                // 1
                // 0 2 3 4 5

                if (child == current || visited[child])
                    continue;
                Q.push(Edge(current, child, adjList[current][child]));
            }
        }
        cout << "Longeur du réseau = " << cost << endl ; 
        return solution;
    }
};

#endif