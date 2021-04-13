/**
* klassenaam: Node klasse. Een Node stelt een knoop uit de expressieboom voor. Het type en de waarde hangt af van de invoer.
* Dennis Buurman (s2027100)
* node.h
* datum laatste wijziging: 1-11-2019
**/

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <cstdlib>
#include <string.h>
#include <fstream> 
#include <sstream>
#include <cmath>
#include <limits>

int const LEN = 3; // length of longest operation/constant name (sin, cos, pi)

class Node
{
public:
	Node(std::string part);
	~Node();

	enum Type
	{
		PLUS = 1, MINUS, MULTIPLY, DIVISION, POWER, VARIABLE, VALUE, SINE, COSINE, PI
	};

	union Content
	{
		char sincospi[LEN];
		char operator_variable;
		double value;
	};

	/**
	* @function: type_value
	* @abstract: Geeft de Node een type en waarde
	* @param: std::string part -> inhoud van de Node
	* @return: -
	* @pre: std::string part is getest door de Tree
	* @post: De Node heeft een waarde en type
	**/
	void type_value(std::string part);

	/**
	* @function: print
	* @abstract: print de waarde van de Node
	* @param: -
	* @return: -
	* @pre: none
	* @post: none
	**/
	void print() const;

	int arity;
	Type t;
	Content c;
	int label;

	Node* left;
	Node* right;
	Node* parent;
private:

};//Node

#endif