/**
* klassenaam: maakt een tree object aan en geeft deze invoer 
* Dennis Buurman (s2027100)
* main.cc
* datum laatste wijziging: 1-11-2019
**/

#include <iostream>
#include <string>

#include "tree.h"

int main(int argc, char* argv[])
{
	bool status = true, debug = false;
	std::string input;
	Tree* tree = new Tree;

	if(argc > 1)
		if (std::string(argv[1]) == "-d")
			debug = true;

	std::cout << debug << std::endl;

	if(debug)
		std::cout << "Choose from: exp <expression>, print, dot <filename>, eval <value>, diff, simp, end:" << std::endl;

	while(status)
	{
		std::getline(std::cin, input);
		status = tree->parseInput(input);
		if(!status && tree->error > 0)
			std::cout << "error code: " << tree->error << " in : " << tree->error_str << std::endl;
	}

	delete tree;
	return 0;
}//main