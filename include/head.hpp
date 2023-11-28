#include <iostream>
#include <stack>
#include <string>
using namespace std;

const int LEN = 301;

class Lexeme
{
private:
	char infix[LEN]; //infix write
	char postfix[LEN]; //postfix write
	stack<double>num; //stack for numbers
	stack<char>op; //stack for operators

	int priority(char symb); //priority of symbols and operations
	bool isOperator(char symb); //checks is symbol math operator
	bool isNumber(char symb); //checks is symbol number

public:
	Lexeme() {}
	~Lexeme() {}
	bool checkSc(char* s); // checks for right scarb order
	void delStack(); //delete all in stack
	void infToPost(); //infix write to postfix one
	double Counter(); //calculator
	double postCounter(); //postfix calculator
	int statusPath();
};