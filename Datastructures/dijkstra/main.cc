/**
* klassenaam: creates a queue and calls methods depending on input 
* Dennis Buurman (s2027100)
* main.cc
* datum laatste wijziging: 5-12-2019
**/

#include "prio.h"
#include "dijkstra.h"

#include <iostream>
#include <string>

bool getParams(int &item, int &prio, std::string line)
{
	std::string items, prios;
	bool delimiter = false;
	unsigned int i = 0;
	while(line[i] != ' ')
		i++;
	i++;
	while(!delimiter && i < line.size())
	{
		if(line[i] == ' ' || line[i] == ',')
			delimiter = true;
		else
			items += line[i];
		i++;
	}
	if(items.size() == 0)
		return false;
	while(i < line.size())
	{
		if(line[i] == ' ' || line[i] == ',')
			return false;
		else
			prios += line[i];
		i++;
	}

	item = atoi(items.c_str());
	prio = atoi(prios.c_str());
	return true;
}

void priorityQueue()
{
	int item = 0, prio = 0;
	Prio<int>* queue = new Prio<int>;

	std::cout << "Choose from: isEmpty, insert <newItem> <prio>, getMinimum, deleteMinimum" << std::endl;

	for (std::string line; std::getline(std::cin, line);)
	{
		if(line.substr(0, line.find(" ")) == "isEmpty")
		{
			if(queue->isEmpty())
				std::cout << "empty" << std::endl;
			else
				std::cout << "not empty" << std::endl;
		}
		else if(line.substr(0, line.find(" ")) == "insert")
		{
			getParams(item, prio, line);
			queue->insert(item, prio);
		}
		else if(line.substr(0, line.find(" ")) == "getMinimum")
		{
			if(!queue->isEmpty())
				std::cout << queue->getMinimum() << std::endl;
		}
		else if(line.substr(0, line.find(" ")) == "deleteMinimum")
			queue->deleteMinimum();
		else if(line.substr(0, line.find(" ")) == "print")
			queue->print();
		else if(line.substr(0, line.find(" ")) == "decreaseKey")
		{
			getParams(item, prio, line);
			queue->decreaseKey(item, prio);
		}
		else if(line.substr(0, line.find(" ")) == "q"){
			delete queue;
			return;
		}
	}

	delete queue;	
}

std::string getFileName()
{
	std::string filename;
	std::cout << "Enter the name of a .grf file (without extension)" << std::endl;
	std::getline(std::cin, filename);
	return filename;
}

void dijkstra()
{
	Dijkstra* dijkstra = new Dijkstra(getFileName());
	dijkstra->algorithm();
}

int main(int argc, char** argv)
{
	if(argc > 1 && std::string(argv[1]) != "-d")
		priorityQueue();
	else
		dijkstra();

	return 0;
}