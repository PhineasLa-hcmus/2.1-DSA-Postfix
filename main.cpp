#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <functional>
#include <math.h>
#include <string.h>

int getPrecedence(char c)
{
	switch (c)
	{
	case '^':
		return 4;
	case '/': case '*':
		return 3;
	case '-': case '+':
		return 2;
	case '(': case ')':
	case '[': case ']':
	case '{': case '}':
		return 1;
	default:
		throw;
	}
}

bool isOperator(char c)
{
	return (c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '^');
}

int isOpenBracket(char c)
{
	switch (c)
	{
	case '(': return 1;
	case '[': return 2;
	case '{': return 3;
	default: return 0;
	}
}

int isCloseBracket(char c)
{
	switch (c)
	{
	case ')': return 1;
	case ']': return 2;
	case '}': return 3;
	default: return 0;
	}
}

bool isFloat(const std::string& str,int pos, int length) {
	if (length == 0) return false;
	if (str[pos] == '.' || str[pos+length-1] == '.') return false;
	int numOfDot = 0;
	int i = pos;
	while (i < pos + length - 1) {
		if (str[i] == '.') numOfDot++;
		if (numOfDot >= 2) return false;
		i++;
	}
	return true;
}

float stringToNum(std::string str, int pos, int length) {
	float num = stof(str.substr(pos, length), nullptr);
	return num;
}

bool changeStrToArray(const std::string& str, std::vector<float>& number, std::vector<char>& legalOperator) {
	int pos = str.length(), length = 0;
	int k;

	if (isOperator(str[0]) || isOperator(str[str.length() - 1]) || isCloseBracket(str[0]) || isOperator(str[str.length() - 1])) return false;

	//If it's a number put it in number array
	for (int i = 0; i < str.length(); i++) {
		for (int j = i; j < str.length(); j++) {
			if (isdigit(str[j]) || str[j] == '.') {
				k = j;
				length++;
				if (pos > k) {
					pos = k;

				}
			}
			if (isOperator(str[i]) || isCloseBracket(str[i]) || isOpenBracket(str[i])) {
				legalOperator.push_back(str[i]);
				if (isOpenBracket(str[i]) || (isCloseBracket(str[i]) && i < str.length() - 1 && isCloseBracket(str[i + 1]))) i = j;
				else
					i = j + 1;
				break;
			}
			if (isCloseBracket(str[j]) || str[j] == ' ' || j == str.length() - 1) {
				if (isFloat(str, pos, length))
					number.push_back(stringToNum(str, pos, length));
				length = 0;
				pos = str.length();
				if (isCloseBracket(str[j])) {

					i = j - 1;
				}
				else
					i = j;
				break;
			}
		}
	}
	return true;
}

bool checkBracket(std::vector<char> legalOperator) {
	std::vector<char> bracketArr;
	//create bracket array
	for (int i = 0; i < legalOperator.size(); i++) {
		if (isOpenBracket(legalOperator[i]) || isCloseBracket(legalOperator[i])) bracketArr.push_back(legalOperator[i]);
	}
	
	int l = 0;
	int r = bracketArr.size() - 1;
	int bracketValue = 4;	//save value of the biggest bracket
	
	while (l < r) {
		if (isOpenBracket(bracketArr[l]) == isCloseBracket(bracketArr[r]) && isOpenBracket(bracketArr[l])<=bracketValue) {
			bracketValue = isOpenBracket(bracketArr[l]);
			l++;
			r--;
		}
		else return false;
	}
	return true;
}

bool checkValidate(const std::string& str) {
	std::vector<float> number;
	std::vector<char> legalOperator;
	if (changeStrToArray(str, number, legalOperator)&&checkBracket(legalOperator)) {
		int numberOfOperator = 0;
		int numOfOpenBracket1 = 0;	//(
		int numOfOpenBracket2 = 0;	//[
		int numOfOpenBracket3 = 0;	//{
		int numOfCloseBracket1 = 0;	//)
		int numOfCloseBracket2 = 0;	//]
		int numOfCloseBracket3 = 0;	//}
	
		for (int i = 0; i < legalOperator.size(); i++) {
			if (isOperator(legalOperator[i])) numberOfOperator++;
			if (legalOperator[i] == '(') numOfOpenBracket1++;
			else if (legalOperator[i] == '[') numOfOpenBracket2++;
			else if (legalOperator[i] == '{') numOfOpenBracket3++;
			else if (legalOperator[i] == ')')
				numOfCloseBracket1++;
			else if (legalOperator[i] == ']') numOfCloseBracket2++;
			else if (legalOperator[i] == '}') numOfCloseBracket3++;
		}
		if ((number.size() == (numberOfOperator + 1))
			&& numOfOpenBracket1 == numOfCloseBracket1
			&& numOfOpenBracket2 == numOfCloseBracket2
			&& numOfOpenBracket3 == numOfCloseBracket3) return true;
	}
	return false;
}

class BinTree
{
public:
	void buildFromInfix(const std::string& str)
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
			else if (str[i + 1] == ' ' /*REDUNDANT, detect negative number*/ && isOperator(str[i]))
			{
				while (!opStack.empty() && ((str[i] != '^' && getPrecedence(opStack.top()) >= getPrecedence(str[i])) || str[i] == '^' && getPrecedence(opStack.top()) > getPrecedence(str[i])))
				{
					pushFromStack();
					//root = postfix.top();
				}
				opStack.push(str[i]);
				++i;
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
		root = postfix.top();
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
		return -1;
	}

	std::vector<std::string> input;
	if (!readFile(argv[1], atoi(argv[2]), input))
	{
		std::cout << "Invalid input file";
		return -1;
	}

	std::function<void(const BinTree &, std::stringstream &)> outputMode;
	if (strcmp(argv[3], "-c") == 0)
	{
		outputMode = [](const BinTree &tree, std::stringstream &ss) {
			ss << tree.calculate() << std::endl;
		};
	}
	else if (strcmp(argv[3], "-t") == 0)
	{
		outputMode = [](const BinTree &tree, std::stringstream &ss) {
			ss << tree.toPostfix() << std::endl;
		};
	}
	else
	{
		std::cout << "Invalid arguments";
		return -1;
	}

	std::stringstream outputStream;
	for (auto i = input.begin(); i != input.end(); ++i)
	{
		if (checkValidate(*i))
		{
			BinTree tree;
			tree.buildFromInfix(*i);
			outputMode(tree, outputStream);
		}
		else
		{
			outputStream << 'E' << std::endl;
		}
	}
	writeFile(argv[4], outputStream);

	// DEBUG
	// std::vector<std::string> input;
	// readFile("input.txt", 16, input);
	// for (auto i = input.begin(); i != input.end(); ++i)
	// {
	// 	if (checkValidate(*i))
	// 	{
	// 		BinTree tree;
	// 		tree.buildFromInfix(*i);
	// 		std::cout << "Infix: " << *i << std::endl;
	// 		std::cout << "Postfix: " << tree.toPostfix() << std::endl;
	// 		std::cout << "Result: " << tree.calculate() << std::endl;
	// 	}
	// }
}