/**
* klassenaam: Tree klasse. Maakt of verandert een expressieboom van Nodes uit de Node klasse afhankelijk van de invoer.
* Dennis Buurman (s2027100)
* tree.h
* datum laatste wijziging: 1-11-2019
**/

#include "node.h"

#include <iostream>
#include <string>
#include <cmath>
#include <limits>

class Tree
{
public:
	Tree();
	~Tree();

	/**
	* @function: parseInput
	* @abstract: parses input and redirects accordingly
	* @param: std::string in -> input to be parsed
	* @return: true if succes and false on error or ending program
	* @pre: none
	* @post: the tree could be manipulated in various ways
	**/
	bool parseInput(std::string in);

	/**
	* @function: checkExpression
	* @abstract: checks if the inputted expression contains valid terms
	* @param: std::string exp -> the expression to be parsed
	* @return: true if succes, false if an unknow term is found
	* @pre: none
	* @post: function word "exp" is deleted from exp string 
	**/
	bool checkExpression(std::string &exp);

	/**
	* @function: validate
	* @abstract: checks for syntax errors in tree
	* @param: Node* node -> Node to start validating from (goes down recursively)
	* @return: true on succes, false on syntax error
	* @pre: none
	* @post: none
	**/
	bool validate(Node* const node) const;

	/**
	* @function: expression
	* @abstract: cuts the inputted expression into parts and feeds them to addPart to build a new tree (old one deleted)
	* @param: std::string exp -> expression that is cut into parts for addPart
	* @return: true on succes, false on syntax error
	* @pre: exp is handled by checkExpression
	* @post: A new tree is build or an error occured
	**/
	bool expression(std::string const exp);

	/**
	* @function: addPart
	* @abstract: adds part string to tree as a new Node
	* @param: std::string part -> part to be added to tree as a new Node
	* @return: true on succes, false on syntax error
	* @pre: part string is checked by checkExpression
	* @post: A new Node is added to the tree or an error occured
	**/
	bool addPart(std::string const part);

	/**
	* @function: print
	* @abstract: prints the content of the Tree if there is one
	* @param: Node* node -> Node to start printing from (recursively)
	* @return: -
	* @pre: none
	* @post: none
	**/
	void print(Node* const node) const;

	/**
	* @function: deleteTree
	* @abstract: removes the current tree, if there is one
	* @param: Node* node -> Node to start deleting from (recursively)
	* @return: -
	* @pre: none
	* @post: expression deleted
	**/
	void deleteTree(Node* node);

	/**
	* @function: dot
	* @abstract: represents the current expression in DOT format
	* @param: std::ofstream output -> output file
	Node* node -> Node to start from (recursively)
	int labelnum -> label number for arrows in DOT
	* @return: -
	* @pre: output must be opened and labelnum must be initiated
	* @post: output file is filled with the DOT notation of the expression
	**/
	void dot(std::ofstream &output, Node* const node, int &labelnum) const;

	/**
	* @function: handleError
	* @abstract: handles error so program can continue
	* @param: -
	* @return: -
	* @pre: -
	* @post: error is reset
	**/
	void handleError();

	/**
	* @function: evaluate
	* @abstract: evaluates x in expression 
	* @param: Node* node -> Node to start from (recursively)
	* @return: true on succes, false on error
	* @pre: -
	* @post: expression is evaluated for x
	**/
	bool evaluate(Node* node);

	/**
	* @function: simplify
	* @abstract: simplifies expression
	* @param: Node* node -> node to start from (recursively)
	* @return: true on succes, false on error
	* @pre: -
	* @post: expression is simplified
	**/
	bool simplify(Node* node);

	/**
	* @function: compute
	* @abstract: computes the subexpression of a node and its children
	* @param: Node* node -> node to compute
	* @return: -
	* @pre: node has two valid children
	* @post: the answer of the subexspression is put into node and its children are deleted
	**/
	void compute(Node* node);

	/**
	* @function: computeVar
	* @abstract: handles x-x and x/x with x a variable
	* @param: Node* node -> subexpression with two variables
	* @return: true on succes, false on failure
	* @pre: operator is checked
	* @post: node is changed on succes
	**/
	bool computeVar(Node* node);

	/**
	* @function: mul_oneZero
	* @abstract: handles multiplication with 1 and 0
	* @param: Node* node -> subexpression with x*0, 0*x, x*1, 1*x with x a variable or value
	* @return: true on succes, false on failure
	* @pre: operator and children are valid (* operator and 1 or 0 as a child)
	* @post: node is changed on succes
	**/
	bool mul_oneZero(Node* node);

	/**
	* @function: pow_oneZero
	* @abstract: handles power operator where right child is 0 or 1
	* @param: Node* node -> subexpression with ^0 or ^1
	* @return: true on succes, false on failure
	* @pre: operator and child valid
	* @post: node is changed on succes
	**/
	bool pow_oneZero(Node* node);

	/**
	* @function: rZero
	* @abstract: handles exp+0 and 0+exp
	* @param: Node* node -> subexpression with a +0
	* @return: true on succes, false on failure
	* @pre: Operator checked
	* @post: +0 removed from expression
	**/
	bool rZero(Node* node);

	/**
	* @function: differentiate
	* @abstract: differentiates to x
	* @param: Node* node -> node to differentiate
	* @return: true on succes, false on error
	* @pre: -
	* @post: expression is replaced
	**/
	bool differentiate(Node* node);

	/**
	* @function: copyTree
	* @abstract: copies a (sub)tree from a node to its leaves
	* @param: Node* node -> node to start copying from
	* @return: root of the copied tree
	* @pre: -
	* @post: -
	**/
	Node* copyTree(Node* const node);

	bool precise;
	double eval;
	int dotlabel;
	int error;
	std::string error_str;
private:
	Node* root;
	Node* index;
	std::string input;
};//Tree