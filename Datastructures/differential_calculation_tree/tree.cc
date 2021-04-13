/**
* klassenaam: implementatie van de klasse Tree
* Dennis Buurman (s2027100)
* tree.cc
* datum laatste wijziging: 1-11-2019
**/

#include "tree.h"

std::string doubleToString(double const in)
{
		std::ostringstream output;
		output << in;
		return output.str();
}//doubleToString

//Machine epsilon for double precision calculations like sin(pi)
void doubleComparison(double &value)
{
	if(value < std::numeric_limits<double>::epsilon() && 
		value > -std::numeric_limits<double>::epsilon())
	{
		value = 0.0;
	}
}//doubleComparison

Tree::Tree()
{
	precise = false;
	eval = 0;
	dotlabel = 1;
	error = 0;
	error_str = "";
	root = nullptr;
	index = nullptr;
}//Tree::Tree

Tree::~Tree()
{
	error_str.clear();
	deleteTree(root);
}//Tree::~Tree

void Tree::handleError()
{
	if(error > 0)
	{
		std::cout << "error code: " << error << " in : " << error_str << std::endl;

		if(error < 4 || error == 6)
		{
			index = nullptr;
			deleteTree(root);
			root = nullptr;
		}

		error = 0;
		error_str.clear();
	}
}//Tree::handleError

bool Tree::checkExpression(std::string &in)
{
	size_t pos = in.find(" ");
	unsigned int j = 0;
	std::string temp = "";

	in.erase(0, pos + 1);

	if(in == "exp")
	{
		error_str = "empty expression";
		return false;
	}

	for(unsigned int i = 0; i < in.length(); i++)
	{
		if(in[i] == 's')
		{
			j = i;
			while(in[j] != ' ' && j < in.length())
			{
				temp += in[j];
				j++;
			}
			if(temp != "sin" && temp.length() > 1)
			{
				error_str = temp;
				return false;
			}
			i += temp.length();
			temp.clear();
		}
		else if(in[i] == 'c')
		{
			j = i;
			while(in[j] != ' ' && j < in.length())
			{
				temp += in[j];
				j++;
			}
			if(temp != "cos" && temp.length() > 1)
			{
				error_str = temp;
				return false;
			}
			i += temp.length();
			temp.clear();
		}
		else if(in[i] == 'p')
		{
			j = i;
			while(in[j] != ' ' && j < in.length())
			{
				temp += in[j];
				j++;
			}
			if(temp != "pi" && temp.length() > 1)
			{
				error_str = temp;
				return false;
			}
			i += temp.length();
			temp.clear();
		}
		else if((in[i] > 96 && in[i] < 123) || (in[i] > 64 && in[i] < 91))
		{
			j = i;
			while(in[j] != ' ' && j < in.length())
			{
				temp += in[j];
				j++;
			}
			if(temp.length() > 1)
			{
				error_str = temp;
				return false;
			}
			i += temp.length();
			temp.clear();
		}
		else if(in[i] > 47 && in[i] < 58)
		{
			j = i;
			while(in[j] != ' ' && j < in.length())
			{
				temp += in[j];
				j++;
			}
			if(atof(temp.c_str()) == 0.0 && temp != "0.0" && temp != "0")
			{
				error_str = temp;
				return false;
			}
			i += temp.length();
			temp.clear();
		}
		else if(in[i] == '+' || in[i] == '*' || in[i] == '/' || in[i] == '^')
		{
			j = i;
			while(in[j] != ' ' && j < in.length())
			{
				temp += in[j];
				j++;
			}
			if(temp.length() > 1)
			{
				error_str = temp;
				return false;
			}
			i += temp.length();
			temp.clear();
		}
		else if(in[i] == '-')
		{
			j = i;
			while(in[j] != ' ' && j < in.length())
			{
				temp += in[j];
				j++;
			}
			if(temp.length() > 1)
			{
				if(atof(temp.c_str()) == 0.0)
				{
					error_str = temp;
					return false;
				}
			}
			i += temp.length();
			temp.clear();
		}
	}

	return true;
}//Tree::checkExpression

bool Tree::addPart(std::string const part)
{
	Node* temp = new Node(part);
	temp->parent = index;

	if(index == nullptr && root == nullptr)
		root = temp;
	if (index != nullptr)
	{
		if(index->arity == 0)
		{ //leaf, variable or value
			while(temp->parent->arity != 1 && temp->parent != nullptr)
			{
				if(temp->parent->parent == nullptr)
					return false; 
				temp->parent = temp->parent->parent;
			}
			temp->parent->right = temp;
			temp->parent->arity -= 1;
		}
		if(index->arity == 1)
		{ //sin cos or right child
			index->right = temp;
			index->arity -= 1;
		}
		if(index->arity == 2)
		{ //operator as parent
			index->left = temp;
			index->arity -= 1;
		}
	}

	index = temp;
	return true;
}//Tree::addPart

bool Tree::expression(std::string const exp)
{
	bool status = true;
	std::string temp = "";

	if(root != nullptr)
	{
		index = nullptr;
		deleteTree(root);
		root = nullptr;
	}
	for (unsigned int i = 0; i <= exp.size(); i++)
	{
		if (exp[i] == ' ' || i == exp.size())
		{
			if (temp != "")
			{
				status = addPart(temp);
				if(!status)
					return false;
				temp.erase();
			}
		}
		else
			temp += exp[i];
	}

	return true;
}//Tree::expression

bool Tree::validate(Node* const node) const
{
	bool l = true, r = true;
	if(node != nullptr)
	{
		if(node->left != nullptr)
			l = validate(node->left);
		if(node->right != nullptr)
			r = validate(node->right);
		if(!l || !r)
			return false;
		if(node->arity != 0)
			return false;
	}
	return true;
}//Tree::validate

bool Tree::parseInput(std::string in)
{
	bool status = true;

	input = in;

	if(in.substr(0, in.find(" ")) == "exp")
	{
		status = checkExpression(in);
		if(!status)
		{
			error = 1; // unknown term
			// return false;
			handleError();
		}
		status = expression(in);
		if(!status)
		{
			error = 2; // try to add part, but no arity left
			error_str = "syntax error";
			// return false;
			handleError();
		}
		status = validate(root);
		if(!status)
		{
			error = 3; // free spots in expression
			error_str = "syntax error";
			// return false;
			handleError();
		}
	}
	else if(in.substr(0, in.find(" ")) == "print")
	{
		print(root);
		std::cout << std::endl;
	}
	else if(in.substr(0, in.find(" ")) == "dot")
	{
		if(in.length() < 9)
		{
			error = 4; // no .dot file found
			error_str = "No '.dot' file given";
			// return false;
			handleError();
		}
		if(in.substr(in.size()-4, in.size()) != ".dot")
		{
			error = 4; // no .dot file found
			error_str = "No '.dot' file given";
			// return false;
			handleError();
		}
		if(root != nullptr)
		{
			size_t pos = in.find(" ");
			std::string temp = in;
			temp.erase(0, pos + 1);

			std::ofstream output;
			output.open(temp.c_str(), std::ios::out);
			output << "digraph G { \n";
			dot(output, root, dotlabel);
			output << "}";
			output.close();
		}
		else
		{
			error = 5; // no expression available
			error_str = "No expression available";
			// return false;
			handleError();
		}
	}
	else if(in.substr(0, in.find(" ")) == "eval")
	{
		std::string temp = input;
		size_t pos = temp.find(" ");
		temp.erase(0, pos + 1);
		eval = atof(temp.c_str());

		status = evaluate(root);

		if(!status)
		{
			error = 5; // no expression available
			error_str = "No expression available";
			handleError();
		}

		simplify(root);
	}
	else if(in.substr(0, in.find(" ")) == "diff")
	{
		simplify(root);
		status = differentiate(root);

		if(!status)
			handleError();

		simplify(root);
	}
	else if(in.substr(0, in.find(" ")) == "simp")
	{
		status = simplify(root);

		if(!status)
			handleError();
	}
	else if(in.substr(0, in.find(" ")) == "end")
	{
		std::cout << "ending program" << std::endl;
		return false;
	}

	return true;
}//Tree::parseInput

bool Tree::evaluate(Node* node)
{
	if(node != nullptr)
	{
		int typecast = (int)node->t; //value in enum
		if(typecast == 6)
		{
			if(node->c.operator_variable == 'x')
				node->type_value(doubleToString(eval));
		}
		if(root->left != nullptr)
			evaluate(node->left);
		if(root->right != nullptr)
			evaluate(node->right);
		return true;
	}
	else
		return false;
}//Tree::evaluate

void Tree::compute(Node* node)
{
	double lv = 0.0, rv = 0.0, ans = 0.0;

	if(node->left != nullptr)
	{
		if((int)node->left->t == 10)
			lv = M_PI;
		else
			lv = node->left->c.value;
	}
	if((int)node->right->t == 10)
			rv = M_PI;
		else
			rv = node->right->c.value;

	switch((int)node->t)
	{
		case 1:
			ans = lv + rv;
			break;
		case 2:
			ans = lv - rv;
			break;
		case 3:
			ans = lv * rv;
			if(lv == 0.0 || rv == 0.0)
				ans = 0.0;
			break;
		case 4:
			if(rv == 0.0)
			{
				error = 6; // Division by zero is undefined
				error_str = "Division by zero undefined";
				return;
			}
			ans = lv / rv;
			if(cos(ans) == cos(M_PI/2))
				precise = true;
			break;
		case 5:
			ans = 1.0;
			for (int i = 0; i < rv; ++i)
			{
				ans *= lv;
			}
			break;
		case 8: // SIN
			if(precise)
			{
				ans = std::sin(M_PI/2);
				precise = false;
			}
			else
				ans = std::sin(rv);
			doubleComparison(ans);
			break;
		case 9: // COS
			if(precise)
			{
				ans = std::cos(M_PI/2);
				precise = false;
			}
			else
				ans = std::cos(rv);
			doubleComparison(ans);
			break;
	}

	node->type_value(doubleToString(ans));
	deleteTree(node->left);
	deleteTree(node->right);
	node->left = nullptr;
	node->right = nullptr;
}//Tree::compute

bool Tree::computeVar(Node* node)
{
	if((int)node->left->t != 6 || (int)node->right->t != 6)
		return false;
	if(node->left->c.operator_variable != node->right->c.operator_variable)
		return false;
	if((int)node->t == 2)
		node->type_value("0.0");
	else
		node->type_value("1.0");

	deleteTree(node->left);
	deleteTree(node->right);
	node->left = nullptr;
	node->right = nullptr;
	return true;
}

bool Tree::mul_oneZero(Node* node)
{
	std::string buf;
	if((int)node->left->t == 7)
	{
		if(node->left->c.value == 0.0)
		{
			node->type_value("0.0");
			deleteTree(node->left);
			deleteTree(node->right);
			node->left = nullptr;
			node->right = nullptr;
			return true;
		}
		else if(node->left->c.value == 1.0)
		{
			buf.push_back(node->right->c.operator_variable);
			node->type_value(buf);
			deleteTree(node->left);
			deleteTree(node->right);
			node->left = nullptr;
			node->right = nullptr;
			return true;
		}
	}
	else if((int)node->right->t == 7)
	{
		if(node->right->c.value == 0.0)
		{
			node->type_value("0.0");
			deleteTree(node->left);
			deleteTree(node->right);
			node->left = nullptr;
			node->right = nullptr;
			return true;
		}
		else if(node->right->c.value == 1.0)
		{
			buf.push_back(node->left->c.operator_variable);
			node->type_value(buf);
			deleteTree(node->left);
			deleteTree(node->right);
			node->left = nullptr;
			node->right = nullptr;
			return true;
		}
	}

	return false;
}//Tree::m_oneZero

bool Tree::pow_oneZero(Node* node)
{
	std::string buf;
	if((int)node->right->t == 7)
	{
		if(node->right->c.value == 0.0)
		{
			node->type_value("1.0");
			deleteTree(node->left);
			deleteTree(node->right);
			node->left = nullptr;
			node->right = nullptr;
			return true;
		}
		else if(node->right->c.value == 1.0)
		{
			Node* temp = node->left;
			temp->parent = node->parent;
			if(temp->parent != nullptr)
			{
				if(temp->parent->left == node)
					temp->parent->left = temp;
				else
					temp->parent->right = temp;
				node->parent = nullptr;
				node->left = nullptr;
				deleteTree(node);
				node = nullptr;
			}
			else
			{
				root = temp;
				node->left = nullptr;
				deleteTree(node);
				node = nullptr;
			}
			return true;
		}
	}
	return false;
}//Tree::p_oneZero

bool Tree::rZero(Node* node)
{
	Node* temp;
	if(node->left->c.value == 0.0)
	{
		temp = node->right;
		temp->parent = node->parent;
		if(node->parent == nullptr)
		{
			root = temp;
			node->right = nullptr;
			deleteTree(node);
			node = nullptr;
		}
		else
		{
			if(node == temp->parent->right)
				node->parent->right = temp;
			else
				node->parent->left = temp;
			node->right = nullptr;
			deleteTree(node);
		}
		return true;
	}
	else if(node->right->c.value == 0.0)
	{
		temp = node->left;
		temp->parent = node->parent;
		if(node->parent == nullptr)
		{
			root = temp;
			node->left = nullptr;
			deleteTree(node);
			node = nullptr;
		}
		else
		{
			if(node == temp->parent->right)
				node->parent->right = temp;
			else
				node->parent->left = temp;
			node->left = nullptr;
			deleteTree(node);
		}
		return true;
	}
	return false;
}

bool Tree::simplify(Node* node)
{
	bool l = false, r = false;

	if(node != nullptr)
	{
		if(node->left != nullptr)
			l = simplify(node->left);
		if(node->right != nullptr)
			r = simplify(node->right);

		if((int)node->t == 7 || (int)node->t == 10)
			return true;
		if((int)node->t > 0 && (int)node->t < 6)
		{
			if((int)node->t == 2 || (int)node->t == 4)
				if((int)node->left->t == 6 || (int)node->right->t == 6)
					return computeVar(node);
			if(l && r)
			{
				compute(node);
				if(error == 6)
					return false;
				return true;
			}
			if((int)node->t == 3)
				return mul_oneZero(node);
			if((int)node->t == 5)
				return pow_oneZero(node);
			if((int)node->t == 1 || (int)node->t == 2)
				return rZero(node);
		}
		else if((int)node->t == 8 || (int)node->t == 9)
			if(r)
			{
				compute(node);
				return true;
			}
		return false;
	}
	else
		return false;
}//Tree::simplify

void Tree::print(Node* const node) const
{
	if(node != nullptr)
	{
		int const type = (int)node->t;
		if(node->left != nullptr)
		{
			if(type != 7 && type != 6)
				std::cout << "(";
			print(node->left);
		}
		node->print();
		if(node->right != nullptr)
		{	
			if(type == 8 || type == 9)
				std::cout << "(";
			print(node->right);
			if(type != 7 && type != 6)
				std::cout << ")";
		}
	}
}//Tree::print

void Tree::deleteTree(Node* node)
{
	if(node != nullptr)
	{
		if(node->left != nullptr)
			deleteTree(node->left);
		if(node->right != nullptr)
			deleteTree(node->right);

		node->left = nullptr;
		node->right = nullptr;
		node->parent = nullptr;
		delete node;
		node = nullptr;
	}
}//Tree::deleteTree

void Tree::dot(std::ofstream &output, Node* const node, int &labelnum) const
{
	node->label = labelnum;
	labelnum += 1;
	int typecast = (int)node->t; //value in enum
	std::string temp;

	if(typecast == 7)
		temp = doubleToString(node->c.value);
	else if(typecast > 0 && typecast < 7)
		temp = node->c.operator_variable;
	else
		temp = node->c.sincospi;

	if(node->parent != nullptr)
	{
		output << node->label << " [label=\"" << temp << "\"]\n";
		output << node->parent->label << "->" << node->label << "\n";
	}
	else
		output << node->label << " [label=\"" << temp << "\"]\n";

	if(node->left != nullptr)
		dot(output, node->left, labelnum);
	if(node->right != nullptr)
		dot(output, node->right, labelnum);
}//Tree::dot

bool Tree::differentiate(Node* node)
{
	simplify(root);
	if(node == nullptr)
	{
		error = 5; // no expression available
		error_str = "No expression available";
		return false;
	}

	if(((int)node->t == 7 || (int)node->t == 10))
		node->type_value("0.0");
	else if((int)node->t == 6)
	{
		if(node->c.operator_variable == 'x')
			node->type_value("1.0");
		else
			node->type_value("0.0");
	}
	else if((int)node->t == 5)
	{
		if((int)node->left->t == 6)
		{
			if(node->left->c.operator_variable == 'x')
			{
				if(node->parent == nullptr)
				{
					Node* mul = new Node("*");
					Node* cons = new Node(doubleToString(node->right->c.value));
					mul->parent = node->parent;
					node->parent = mul;
					if(mul->parent == nullptr)
						root = mul;
					else if(mul->parent->right == node)
						mul->parent->right = mul;
					else
						mul->parent->left = mul;
					mul->left = cons;
					mul->right = node;
					cons->parent = mul;
					node->right->c.value--;
				}
				else
				{
					if((int)node->parent->left->t == 7)
						node->parent->left->c.value *= node->right->c.value;
					else
					{
						Node* mult = new Node("*");
						Node* r = new Node(doubleToString(node->right->c.value));
						mult->parent = node->parent;
						mult->left = node->parent->left;
						mult->parent->left = mult;
						mult->left->parent = mult;
						mult->right = r;
						r->parent = mult;
					}
					node->right->c.value--;
				}
			}
		}
		else
		{
			Node* copy = copyTree(node->left);
			Node* mul = new Node("*");
			if(node->parent == nullptr)
				root = mul;
			else if(mul->parent->right == node)
				mul->parent->right = mul;
			else
				mul->parent->left = mul;
			mul->parent = node->parent;
			mul->right = node;
			mul->left = copy;
			copy->parent = mul;
			node->parent = mul;
			differentiate(copy);

			if((int)node->parent->left->t == 7)
				node->parent->left->c.value *= node->right->c.value;
			else
			{
				Node* mul1 = new Node("*");
				mul1->parent = mul;
				node->parent = mul1;
				mul->right = mul1;
				mul1->right = node;
				mul1->left = copyTree(node->right);
				node->parent = mul1;
			}
			node->right->c.value--;
		}
	}
	else if((int)node->t == 8) //SIN
	{
		if((int)node->right->t == 6)
		{
			if(node->right->c.operator_variable == 'x')
			{
				node->type_value("cos");
			}
		}
		else
		{
			Node* copy = copyTree(node->right);
			Node* mul = new Node("*");
			if(node->parent == nullptr)
				root = mul;
			else if(mul->parent->right == node)
				mul->parent->right = mul;
			else
				mul->parent->left = mul;
			mul->parent = node->parent;
			mul->right = node;
			mul->left = copy;
			copy->parent = mul;
			node->parent = mul;
			differentiate(copy);
			node->type_value("cos");
		}
	}
	else if((int)node->t == 9) //COS
	{
		if((int)node->right->t == 6)
		{
			if(node->right->c.operator_variable == 'x')
			{
				Node* mul = new Node("*");
				Node* nega = new Node("-1");
				node->type_value("sin");
				mul->parent = node->parent;
				node->parent = mul;
				if(mul->parent == nullptr)
					root = mul;
				else if(mul->parent->right == node)
					mul->parent->right = mul;
				else
					mul->parent->left = mul;
				mul->left = nega;
				mul->right = node;
				nega->parent = mul;
			}
		}
		else
		{
			Node* copy = copyTree(node->right);
			Node* mul = new Node("*");
			if(node->parent == nullptr)
				root = mul;
			else if(mul->parent->right == node)
				mul->parent->right = mul;
			else
				mul->parent->left = mul;
			mul->parent = node->parent;
			mul->right = node;
			mul->left = copy;
			copy->parent = mul;
			node->parent = mul;
			differentiate(copy);

			Node* mul1 = new Node("*");
			Node* nega = new Node("-1");
			node->type_value("sin");
			mul1->parent = mul;
			node->parent = mul1;
			mul->right = mul1;
			mul1->left = nega;
			mul1->right = node;
			nega->parent = mul1;
		}
	}
	else if((int)node->t == 1 || (int)node->t == 2)
	{
		differentiate(node->left);
		differentiate(node->right);
	}
	else if((int)node->t == 3)
	{
		if(((int)node->left->t == 6 && node->left->c.operator_variable == 'x') || (((int)node->right->t == 6) && node->right->c.operator_variable == 'x'))
		{
			std::string buf;
			if(node->left->c.operator_variable == 'x')
			{
				if((int)node->right->t == 6)
				{
					buf.push_back(node->right->c.operator_variable);
					node->type_value(buf);
				}
				else
					node->type_value(doubleToString(node->right->c.value));
				deleteTree(node->left);
				deleteTree(node->right);
				node->left = nullptr;
				node->right = nullptr;
			}
			else if(node->right->c.operator_variable == 'x')
			{
				if((int)node->left->t == 6)
				{
					buf.push_back(node->left->c.operator_variable);
					node->type_value(buf);
				}
				else
					node->type_value(doubleToString(node->left->c.value));
				deleteTree(node->left);
				deleteTree(node->right);
				node->left = nullptr;
				node->right = nullptr;
			}
		}
		else
		{
			Node* plus = new Node("+");
			Node* copy = copyTree(node);
			plus->parent = node->parent;
			if(plus->parent == nullptr)
				root = plus;
			else if(plus->parent->right == node)
				plus->parent->right = plus;
			else
				plus->parent->left = plus;
			node->parent = plus;
			copy->parent = plus;
			plus->left = node;
			plus->right = copy;
			differentiate(node->left);
			differentiate(copy->right);
		}
	}
	else if((int)node->t == 4)
	{
		Node* min = new Node("-");
		Node* mul = new Node("*");
		Node* mul1 = new Node("*");
		Node* pow = new Node("^");
		Node* two = new Node("2");
		Node* df = copyTree(node->left);
		Node* dg = copyTree(node->right);
		Node* g = copyTree(node->right);

		min->parent = node;
		min->left = mul;
		min->right = mul1;
		mul->parent = min;
		mul1->parent = min;
		mul->left = df;
		df->parent = mul;
		mul->right = node->right;
		node->right->parent = mul;
		mul1->left = node->left;
		node->left->parent = mul1;
		mul1->right = dg;
		dg->parent = mul1;
		node->left = min;
		pow->left = g;
		g->parent = pow;
		pow->right = two;
		two->parent = pow;
		node->right = pow;
		pow->parent = node;

		differentiate(df);
		differentiate(dg);
	}

	return true;
}//Tree::differentiate

Node* Tree::copyTree(Node* const node)
{
	std::string buf;
	Node* copyroot = new Node("0.0");

	if((int)node->t < 7)
	{
		buf.push_back(node->c.operator_variable);
		copyroot->type_value(buf);
	}
	else if((int)node->t == 7)
		copyroot->type_value(doubleToString(node->c.value));
	else
		copyroot->type_value(node->c.sincospi);

	if(node->left != nullptr)
	{
		copyroot->left = copyTree(node->left);
		copyroot->left->parent = copyroot;
	}
	if(node->right != nullptr)
	{
		copyroot->right = copyTree(node->right);
		copyroot->right->parent = copyroot;
	}

	return copyroot;
}//Tree::copy