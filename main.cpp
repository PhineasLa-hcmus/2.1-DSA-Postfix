#include <iostream>
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

int isParenthesis(char c)
{
	if (c == '(' || c == '[' || c == '{')
		return 1;
	if (c == ')' || c == ']' || c == '}')
		return 2;
	return 0;
}

class BinTree
{
public:
	void buildFromInfix(std::string str)
	{
		std::stack<Node*> postFix;
		std::stack<char> opStack;
		auto pushFromStack = [&]() {
			Node* right = postFix.top();
			postFix.pop();
			Node* left = postFix.top();
			postFix.pop();
			root = new Operator(opStack.top(), left, right);
			postFix.push(root);
			opStack.pop();
		};
		for (size_t i = 0; i < str.length(); ++i)
		{
			if (str[i] == ' ')
				continue;
			int parenthesisType = isParenthesis(str[i]);
			if (parenthesisType == 1)
			{
				opStack.push(str[i]);
			}
			else if (parenthesisType == 2)
			{
				while (isParenthesis(opStack.top()) != 1)
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
		return root->toString();
	}

	float calculate()
	{
		return root->calculate();
	}

private:
	class Node
	{
	public:
		Node* left = nullptr;
		Node* right = nullptr;
		Node() = default;
		Node(Node* left, Node* right)
			: left(left), right(right)
		{}
		virtual float calculate() = 0;
		virtual std::string toString() = 0;
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
		{}
		float calculate()
		{
			return num;
		}
		std::string toString()
		{
			return std::to_string(num);
		}

	private:
		float num;
	};
	class Operator : public Node
	{
	public:
		Operator(char op, Node* left, Node* right)
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

	private:
		char op;
	};
	Node* root = nullptr;
};

int main()
{
	BinTree tree;
	std::string infix = "(-1) + 2 ^ (2) ^ 3";
	tree.buildFromInfix(infix);
	std::cout << tree.toPostfix() << std::endl;
	std::cout << "Infix: " << infix << std::endl;
	std::cout << "Result: " << tree.calculate() << std::endl;
}