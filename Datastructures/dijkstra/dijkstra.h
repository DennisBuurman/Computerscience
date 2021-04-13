/**
* klassenaam: A class containing Dijkstra's algorithm, a priority queue, and a distance array
* Dennis Buurman (s2027100)
* dijkstra.h
* datum laatste wijziging: 5-12-2019
*
**/

#ifndef dijkstra_h
#define dijkstra_h

#include "prio.h"

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <utility>

int static const INT_MAX = 2147483647;
int static const MAX_NODES = 1024;

class Dijkstra
{
public:
	Dijkstra(std::string str);
	~Dijkstra();

	/**
	* @function: getNodes
	* @abstract: finds the amount of nodes from filename.grf
	* @param: -
	* @return: -
	**/
	void getNodes();

	/**
	* @function: initialize
	* @abstract: initializes the dist array
	* @param: -
	* @return: -
	**/
	void initialize();

	/**
	* @function: parse
	* @abstract: parses a line from a .grf file
	* @param: int u, v, w -> represent values of an edge (start, destination, weight)
	* std::string in -> line to be parsed
	* @return: the values of u, v and w are changed
	**/
	void parse(int &u, int &v, int &w, std::string const in);

	/**
	* @function: algorithm
	* @abstract: the algorithm of Dijkstra with output to filename.dis
	* @param: -
	* @return: -
	**/
	void algorithm();

private:
	int node_amount;
	int dist[MAX_NODES];
	std::string filename;
	Prio<int>* queue = new Prio<int>;
};

#endif