#include "head.hpp"

int Lexeme::priority(char symb)
{
	int n=-1;
	if (symb=='(' || symb == ')')
	{
		n = 0;
	}
	else if (symb == '+' || symb == '-')
	{
		n = 1;
	}
	else if (symb == '*' || symb == '/')
	{
		n = 2;
	}
	else
	{
		n = 3;
	}
	return n;
}

bool Lexeme::isNumber(char symb)
{
	if (symb >= 48 && symb <= 57)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Lexeme::isOperator(char symb)
{
	if (symb == '+' || symb == '-' || symb == '*' || symb == '/')
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Lexeme::delStack()
{
	while (!num.empty()) 
	{
		num.pop();
	}
	while (!op.empty())
	{
		op.pop();
	}
}