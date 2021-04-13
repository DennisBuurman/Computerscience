/**
* klassenaam: implementatie van de klasse Node
* Dennis Buurman (s2027100)
* node.cc
* datum laatste wijziging: 1-11-2019
**/

#include "node.h"

Node::Node(std::string part)
{
	left = nullptr;
	right = nullptr;
	parent = nullptr;

	arity = -1;
	t = VALUE;
	c.value = -1.0;
	label = -1;
	
	type_value(part);
}//Node::Node

Node::~Node()
{

}//Node::~Node

void Node::type_value(std::string part)
{
	char var = '!'; //to initialize the variable;
	double const test = atof(part.c_str());

	if (part.length() == 1)
		var = part[0];

	if (var >= 65 && var <= 122)
	{
		t = VARIABLE;
		arity = 0;
		c.operator_variable = part[0];
	}

	if (part == "+")
	{
		t = PLUS;
		arity = 2;
		c.operator_variable = part[0];
	}
	else if (part == "-")
	{
		t = MINUS;
		arity = 2;
		c.operator_variable = part[0];
	}
	else if (part == "/")
	{
		t = DIVISION;
		arity = 2;
		c.operator_variable = part[0];
	}
	else if (part == "*")
	{
		t = MULTIPLY;
		arity = 2;
		c.operator_variable = part[0];
	}
	else if (part == "^")
	{
		t = POWER;
		arity = 2;
		c.operator_variable = part[0];
	}
	else if (part == "sin" || part == "SIN")
	{
		t = SINE;
		arity = 1;
		strcpy(c.sincospi, part.c_str());
	}
	else if (part == "cos" || part == "COS")
	{
		t = COSINE;
		arity = 1;
		strcpy(c.sincospi, part.c_str());
	}
	else if (part == "pi" || part == "PI")
	{
		t = PI;
		arity = 0;
		strcpy(c.sincospi, part.c_str());
	}
	else if (test != 0.0 || part == "0" || part == "0.0")
	{
		t = VALUE;
		arity = 0;
		c.value = test;
	}
}//Tree::type

void Node::print() const
{
	switch(t)
	{
		case PLUS: case MINUS: case DIVISION: case MULTIPLY: case POWER: case VARIABLE:
			std::cout << c.operator_variable;
			break;
		case SINE: case COSINE: case PI:
			std::cout << c.sincospi;
			break;
		case VALUE:
			std::cout << c.value;
			break;
		default:
			std::cout << "unknown symbol" << std::endl;
			break;
	}
}//Node::print