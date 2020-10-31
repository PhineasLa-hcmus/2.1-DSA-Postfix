#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool isDigit(char s);
bool isOperator(char s);
bool isOpenBracket(char s);
bool isCloseBracket(char s);
bool isFloat(string str, int pos, int length);

float stringToNum(string str, int pos, int length) {
	float num = stof(str.substr(pos, length),nullptr);
	return num;
}

bool changeStrToArray(string str, vector<float>& number, vector<char>& legalOperator) {
	int pos = str.length(), length = 0;
	int k;

	if (isOperator(str[0]) || isOperator(str[str.length() - 1]) || isCloseBracket(str[0]) || isOperator(str[str.length() - 1])) return false;

	//If it's a number put it in number array
	for (int i = 0; i < str.length(); i++) {
		for (int j = i; j < str.length(); j++) {
			if (isDigit(str[j]) || str[j] == '.') {
				k = j;
				length++;
				if (pos > k) {
					pos = k;

				}
			}
			if (isOperator(str[i]) || isCloseBracket(str[i]) || isOpenBracket(str[i])) {
				legalOperator.push_back(str[i]);
				if (isOpenBracket(str[i])) i = j;
				else
					i = j + 1;
				break;
			}
			if (isCloseBracket(str[j]) || str[j] == ' ' || j == str.length() - 1) {
				if (isFloat(str, pos, length))
					number.push_back(stringToNum(str, pos, length));
				else return false;
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

bool isDigit(char s) {
	if ((int)s >= 48 && (int)s <= 57) return true;
	return false;
}

bool isOperator(char s) {
	if (s == '*' || s == '/' || s == '+' || s == '-' || s == '^') return true;
	return false;
}

bool isOpenBracket(char s) {
	if (s == '(' || s == '[' || s == '{') return true;
	return false;
}

bool isCloseBracket(char s) {
	if (s == ')' || s == ']' || s == '}') return true;
	return false;
}

bool checkValidate(std::string str) {
	std::vector<float> number;
	std::vector<char> legalOperator;
	if (changeStrToArray(str, number, legalOperator)) {
		int numberOfOperator = 0;
		int numOfOpenBracket1 = 0;	//(
		int numOfOpenBracket2 = 0;	//[
		int numOfOpenBracket3 = 0;	//{
		int numOfCloseBracket1 = 0;	//)
		int numOfCloseBracket2 = 0;	//]
		int numOfCloseBracket3 = 0;	//}
	
		for (int i = 0; i < legalOperator.size(); i++) {
			cout << legalOperator[i] << endl;
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


int main() {
	string str;
	vector<float> number;
	vector<char> legalOperator;

	do {
		cout << "\nstring: ";
		getline(cin, str);
		cout << checkValidate(str) << endl;
		/*for (int i = 0; i < number.size(); i++) cout << number[i] << " ";
		cout << endl;*/
		number.clear();
		legalOperator.clear();
	} while (str != "0");

	/*changeStrToArray(str, number, legalOperator);

	for (int i = 0; i < number.size(); i++) cout << number[i] << " ";
	cout << endl;
	for (int i = 0; i < legalOperator.size(); i++) cout << legalOperator[i] << " ";*/

	return 0;
}