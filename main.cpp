#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <math.h>

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
		std::stack<Node *> postFix;
		std::stack<char> opStack;
		auto pushFromStack = [&]() {
			Node *right = postFix.top();
			postFix.pop();
			Node *left = postFix.top();
			postFix.pop();
			root = new Operator(opStack.top(), left, right);
			postFix.push(root);
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
			else if (str[i + 1] == ' ' && isOperator(str[i]))
			{
				while (!opStack.empty() 
					&& ((str[i] != '^' && getPrecedence(opStack.top()) >= getPrecedence(str[i])) 
						|| str[i] == '^' && getPrecedence(opStack.top()) > getPrecedence(str[i])))
				{
					pushFromStack();
					root = postFix.top();
				}
				opStack.push(str[i]);
			}
			else
			{
				size_t afterNum = 0;
				postFix.push(new Operand(std::stof(str.substr(i), &afterNum)));
				i += afterNum - 1;
			}
		}
		while (!opStack.empty())
		{
			pushFromStack();
		}
	}
	std::string toPostfix()
	{
		std::stringstream ss;
		root->toStream(ss);
		return ss.str();
	}

	float calculate()
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
		{}
		virtual float calculate() = 0;
		virtual std::string toString() = 0;
		virtual void toStream(std::stringstream &) = 0;
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
		float calculate()
		{
			return num;
		}
		std::string toString()
		{
			return std::to_string(num);
		}
		void toStream(std::stringstream &ss)
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
		float calculate()
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
		std::string toString()
		{
			return left->toString() + ' ' + right->toString() + ' ' + op;
		}
		void toStream(std::stringstream &ss)
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

bool writeFile(const char *dir, const std::vector<std::string> output)
{
	std::ofstream fout(dir);
	if (!fout.is_open())
		return false;
	for (auto it = output.begin(); it != output.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
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
	//readFile(argv[1], atoi(argv[2]), input);
	readFile("input.txt", 3, input);

	BinTree tree;
	std::string infix = "1.2 + 2 * 3";
	tree.buildFromInfix(infix);
	std::cout << "Infix: " << infix << std::endl;
	std::cout << "Postfix: " << tree.toPostfix() << std::endl;
	std::cout << "Result: " << tree.calculate() << std::endl;
}