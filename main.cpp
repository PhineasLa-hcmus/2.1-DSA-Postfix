#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <math.h>
#include <string.h>

bool isOperator(char c)
{
	return (c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '^');
}

int getPrecedence(char c)
{
	switch (c)
	{
	case '^':
		return 4;
	case '/':
	case '*':
		return 3;
	case '-':
	case '+':
		return 2;
	case '(':
	case ')':
		return 1;
	default:
		throw;
	}
}

int isOpenBracket(char c)
{
	if (c == '(')
		return 1;
	if (c == '[')
		return 2;
	if (c == '{')
		return 3;
	return 0;
}

int isCloseBracket(char c)
{
	if (c == ')')
		return 1;
	if (c == ']')
		return 2;
	if (c == '}')
		return 3;
	return 0;
}

class BinTree
{
public:
	void buildFromInfix(std::string str)
	{
		std::stack<Node *> postfix;
		std::stack<char> opStack;
		//lambdas function, pop from opStack to build new sub-tree, then push to postfix
		auto pushFromStack = [&]() {
			Node *right = postfix.top();
			postfix.pop();
			Node *left = postfix.top();
			postfix.pop();
			root = new Operator(opStack.top(), left, right); //create tree
			postfix.push(root);
			opStack.pop();
		};
		for (size_t i = 0; i < str.length(); ++i)
		{
			if (str[i] == ' ')
				continue;
			if (isOpenBracket(str[i]))
			{
				opStack.push(str[i]);
			}
			else if (isCloseBracket(str[i]))
			{
				while (!isOpenBracket(opStack.top()))
				{
					pushFromStack();
				}
				opStack.pop(); // discard '('
			}
			else if (str[i + 1] == ' ' /*REDUNDANT, dectect negative number*/ && isOperator(str[i]))
			{
				while (!opStack.empty() && ((str[i] != '^' && getPrecedence(opStack.top()) >= getPrecedence(str[i])) || str[i] == '^' && getPrecedence(opStack.top()) > getPrecedence(str[i])))
				{
					pushFromStack();
					root = postfix.top();
				}
				opStack.push(str[i]);
			}
			else //push a number to postfix, pending to create a tree
			{
				size_t afterNum = 0;
				postfix.push(new Operand(std::stof(str.substr(i), &afterNum)));
				i += afterNum - 1;
			}
		}
		//push all remaining operator(s) from opStack to postfix
		while (!opStack.empty())
		{
			pushFromStack();
		}
	}

	std::string toPostfix() const
	{
		std::stringstream ss;
		root->toStream(ss);
		return ss.str();
	}

	float calculate() const
	{
		return root->calculate();
	}

private:
	class Node
	{
	public:
		Node *left = nullptr;
		Node *right = nullptr;
		Node() = default;
		Node(Node *left, Node *right)
			: left(left), right(right)
		{
		}
		virtual float calculate() const = 0;
		virtual std::string toString() const = 0;
		virtual void toStream(std::stringstream &) const = 0;
		~Node()
		{
			delete left;
			delete right;
		}
	};
	class Operand : public Node
	{
	public:
		Operand(float num)
			: num(num)
		{
		}
		float calculate() const
		{
			return num;
		}
		std::string toString() const
		{
			return std::to_string(num);
		}
		void toStream(std::stringstream &ss) const
		{
			ss << num;
		}

	private:
		float num;
	};
	class Operator : public Node
	{
	public:
		Operator(char op, Node *left, Node *right)
			: op(op), Node(left, right)
		{
		}
		float calculate() const
		{
			switch (op)
			{
			case '+':
				return left->calculate() + right->calculate();
			case '-':
				return left->calculate() - right->calculate();
			case '*':
				return left->calculate() * right->calculate();
			case '/':
				return left->calculate() / right->calculate();
			case '^':
				return pow(left->calculate(), right->calculate());
			default:
				throw;
			}
		}
		std::string toString() const
		{
			return left->toString() + ' ' + right->toString() + ' ' + op;
		}
		void toStream(std::stringstream &ss) const
		{
			left->toStream(ss);
			ss << ' ';
			right->toStream(ss);
			ss << ' ' << op;
		}

	private:
		char op;
	};
	Node *root = nullptr;
};

bool readFile(const char *dir, int numLine, std::vector<std::string> &readFromFile)
{
	std::ifstream fin(dir);
	if (!fin.is_open())
		return false;
	std::string buffer;
	for (int i = 0; std::getline(fin, buffer) && i < numLine; ++i)
	{
		readFromFile.push_back(buffer);
		std::cout << buffer << std::endl;
	}
	return true;
}

bool writeFile(const char *dir, const std::stringstream &stream)
{
	std::ofstream fout(dir);
	if (!fout.is_open())
		return false;
	fout << stream.str();
	return true;
}

int main(int argc, char *argv[])
{
	if (argc < 5)
	{
		std::cout << "Invalid arguments";
		//return -1;
	}

	std::vector<std::string> input;
	if (!readFile(argv[1], atoi(argv[2]), input))
	{
		std::cout << "Invalid input file";
		return -1;
	}

	std::stringstream outputStream;
	if (strcmp(argv[3], "-c") == 0)
	{
		for (auto i = input.begin(); i != input.end(); ++i)
		{
			BinTree tree;
			tree.buildFromInfix(*i);
			outputStream << tree.calculate() << std::endl;
		}
		writeFile(argv[4], outputStream);
	}
	else if (strcmp(argv[3], "-t") == 0)
	{
		for (auto i = input.begin(); i != input.end(); ++i)
		{
			BinTree tree;
			tree.buildFromInfix(*i);
			outputStream << tree.toPostfix() << std::endl;
		}
		writeFile(argv[4], outputStream);
	}
	else
	{
		std::cout << "Invalid arguments";
		return -1;
	}

	// DEBUG
	// readFile("input.txt", 3, input);
	// BinTree tree;
	// std::string infix = "1.2 + 2 * 3";
	// tree.buildFromInfix(infix);
	// std::cout << "Infix: " << infix << std::endl;
	// std::cout << "Postfix: " << tree.toPostfix() << std::endl;
	// std::cout << "Result: " << tree.calculate() << std::endl;
}