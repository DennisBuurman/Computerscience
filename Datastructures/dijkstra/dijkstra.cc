/**
* klassenaam: implementation of methods from dijkstra.h
* Dennis Buurman (s2027100)
* dijkstra.cc
* datum laatste wijziging: 5-12-2019
**/

#include "dijkstra.h"

Dijkstra::Dijkstra(std::string str)
{
	filename = str;
	node_amount = 0;
}

Dijkstra::~Dijkstra()
{
	delete queue;
}

void Dijkstra::getNodes()
{
	std::string line;
	bool nodes = false;

	std::cout << "Reading file: " << filename + ".grf" << std::endl;

	std::ifstream infile(filename + ".grf");
	if(!infile)
	{
		std::cout << "Can't open " << filename + ".grf" << std::endl;
		exit(1);
	}
	while(!nodes)
	{
		std::getline(infile, line);
		if(line[0] != '%')
		{
			node_amount = std::atoi(line.c_str());
			nodes = true;
			std::cout << "Amount of nodes is " << node_amount << std::endl;
		}
	}
	if(node_amount > MAX_NODES)
	{
		std::cout << "Maximum amount of nodes exceeded" << std::endl;
		exit(1);
	}

	infile.close();
}

void Dijkstra::initialize()
{
	for(int i = 0; i <= node_amount; i++)
		dist[i] = INT_MAX;
}

void Dijkstra::parse(int &u, int &v, int &w, std::string const in)
{
	unsigned int j = 0;
	int edge[3];
	std::string temp = "";

	for(int i = 0; i < 3; i++)
	{
		while(in[j] == ' ' && j < in.length())
			j++;
		while(in[j] != ' ' && j < in.length())
		{
			temp += in[j];
			j++;
		}
		edge[i] = std::stoi(temp);
		temp.clear();
	}

	u = edge[0];
	v = edge[1];
	w = edge[2];
}

void Dijkstra::algorithm()
{
	std::string line;
	int u, v, w;
	int start = 0;
	bool found = false;

	getNodes();
	initialize();
	std::cout << "Enter a starting node: ";
	std::getline(std::cin, line);
	std::cout << std::endl;

	if(line.length() == 0)
		return;

	start = std::stoi(line);

	if(start < 1 || start > node_amount)
		return;

	std::list<std::pair<int, int>> graph[MAX_NODES];

	std::ifstream infile(filename + ".grf");

	while(!found)
	{
		std::getline(infile, line);
		if(line[0] != '%')
			found = true;
	}
	while(std::getline(infile, line))
	{
		if(line[0] != '%')
		{
			parse(u, v, w, line);
			graph[u].push_back(std::make_pair(v, w));
			graph[v].push_back(std::make_pair(u, w));
		}
	}

	infile.close();
	dist[start] = 0;
	queue->insert(start, 0);

	while(!queue->isEmpty())
	{
		u = queue->getMinimum();
		queue->deleteMinimum();

		for(std::list<std::pair<int, int>>::iterator i = graph[u].begin(); i != graph[u].end(); i++)
		{
			v = (*i).first;
			w = (*i).second;

			if(dist[v] > dist[u] + w)
			{
				dist[v] = dist[u] + w;
				queue->insert(v, dist[v]);
			}
		}
	}
	// for(int i = 1; i <= node_amount; i++)
	// 	std::cout << i << ", " << dist[i] << std::endl;
	std::ofstream output(filename + ".dis");
	if(output.is_open())
	{
		for(int i = 1; i <= node_amount; i++)
		{
			line.clear();
			line += std::to_string(i) + ", " + std::to_string(dist[i]);
			output << line << std::endl;
		}
	}
	else 
		std::cout << "Unable to open file" << std::endl;
}