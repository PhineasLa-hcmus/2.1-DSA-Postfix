#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool isDigit(char s);
bool isOperator(char s);
int isOpenBracket(char s);
int isCloseBracket(char s);
bool checkBracket(vector<char> legalOperator);
bool isFloat(string str, int pos, int length);

float stringToNum(string str, int pos, int length) {
	float num = stof(str.substr(pos, length), nullptr);
	return num;
}

bool changeStrToArray(string str, vector<float>& number, vector<char>& legalOperator) {
	int pos = str.length(), length = 0;
	int k;

	if (isOperator(str[0]) || isOperator(str[str.length() - 1]) || isCloseBracket(str[0])!=-1 || isOperator(str[str.length() - 1])) return false;

	for (int i = 0; i < str.length(); i++) {
		for (int j = i; j < str.length(); j++) {
			if (isDigit(str[j]) || str[j] == '.') {
				k = j;
				length++;
				if (pos > k) {
					pos = k;

				}
			}
			if (isOperator(str[i]) || isCloseBracket(str[i])!=-1 || isOpenBracket(str[i])!=-1) {
				legalOperator.push_back(str[i]);
				if (isOpenBracket(str[i])!=-1 || (isCloseBracket(str[i])!=-1 && i < str.length() - 1 && isCloseBracket(str[i + 1])!=-1)) i = j;
				else
					i = j + 1;
				break;
			}
			if (isCloseBracket(str[j]) !=-1|| str[j] == ' ' || j == str.length() - 1) {
				if (isFloat(str, pos, length))
					number.push_back(stringToNum(str, pos, length));
				length = 0;
				pos = str.length();
				if (isCloseBracket(str[j])!=-1) {

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


bool isFloat(string str,int pos, int length) {
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

bool isDigit(char s) {
	if ((int)s >= 48 && (int)s <= 57) return true;
	return false;
}

bool isOperator(char s) {
	if (s == '*' || s == '/' || s == '+' || s == '-' || s == '^') return true;
	return false;
}

int isOpenBracket(char s) {
	switch (s) {
	case '(':return 1;
	case '[':return 2;
	case '{':return 3;
	default: return -1;
	}
}

int isCloseBracket(char s) {
	switch (s) {
	case ')':return 1;
	case ']':return 2;
	case '}':return 3;
	default: return -1;
	}
}

//check order of brackets
bool checkBracket(vector<char> legalOperator) {
	vector<char> bracketArr;
	//create bracket array
	for (int i = 0; i < legalOperator.size(); i++) {
		if (isOpenBracket(legalOperator[i]) != -1 || isCloseBracket(legalOperator[i]) != -1) bracketArr.push_back(legalOperator[i]);
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

bool checkValidate(string str, vector<float>& number, vector<char>& legalOperator) {
	if (changeStrToArray(str, number, legalOperator) && checkBracket(legalOperator)) {
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
		cout << checkValidate(str, number, legalOperator) << endl;
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