#include "head.hpp"

bool CheckScobe(string s)
{
	return true;
}

Lexeme GetLexeme(const string& str, int& index, Status status) {
	string s;
	if (str[index] == '*' || str[index] == '/')
	{
		s.push_back(str[index++]);
		return Lexeme({ LexemeType::binary, Priority::mult, s});
	}
	else if (str[index] == '+' || str[index] == '-')
	{
		s.push_back(str[index++]);
		return Lexeme({ LexemeType::binary, Priority::plus, s});
	}
	else if (str[index] == '(')
	{
		s.push_back(str[index++]);
		return Lexeme({ LexemeType::lsc, Priority::scobe, s});
	}
	else if (str[index] == ')')
	{
		s.push_back(str[index++]);
		return Lexeme({ LexemeType::rsc, Priority::scobe, s});
	}
	else if (str[index] >= '0' && str[index] <= '9')
	{
		s.push_back(str[index++]);
		return Lexeme({ LexemeType::num, Priority::num, s });
	}
	else
	{
		return Lexeme{ LexemeType::unknown, Priority::plus, s = "" };
	}
}

vector<Lexeme> StringToLexeme(string str)
{
	int k = 0;
	int ind = 0;
	vector<Lexeme> v;
	Status stat = Status::StartStatus;
	while (ind != str.size())
	{
		const Lexeme lexeme = GetLexeme(str, ind, stat);
		if (stat == Status::StartStatus)
		{
			if (lexeme.type == LexemeType::num || lexeme.type == LexemeType::diff)
			{
				stat = Status::DifferentStatus;
			}
			else if (lexeme.type == LexemeType::lsc)
			{
				stat = Status::LScobeStatus;
				k++;
			}
			else
			{
				stat = Status::ErrorStatus;
			}
		}
		else if (stat == Status::BinaryStatus)
		{
			if (lexeme.type == LexemeType::diff || lexeme.type == LexemeType::num)
			{
				stat = Status::DifferentStatus;
			}
			else if (lexeme.type == LexemeType::lsc)
			{
				stat = Status::LScobeStatus;
				k++;
			}
			else
			{
				stat = Status::ErrorStatus;
			}
		}
		else if (stat == Status::DifferentStatus)
		{
			if (lexeme.type == LexemeType::binary)
			{
				stat = Status::BinaryStatus;
			}
			else if (lexeme.type == LexemeType::rsc)
			{
				stat = Status::RScobeStatus;
				k++;
			}
			else
			{
				stat = Status::ErrorStatus;
			}
		}
		else if (stat == Status::LScobeStatus)
		{
			if (lexeme.type == LexemeType::lsc)
			{
				stat = Status::LScobeStatus;
				k++;
			}
			else if (lexeme.type == LexemeType::diff || lexeme.type == LexemeType::num)
			{
				stat = Status::DifferentStatus;
			}
			else
			{
				stat = Status::ErrorStatus;
			}
		}
		else if (stat == Status::RScobeStatus)
		{
			if (lexeme.type == LexemeType::rsc)
			{
				stat = Status::RScobeStatus;
				k--;
			}
			else if (lexeme.type == LexemeType::binary)
			{
				stat = Status::BinaryStatus;
			}
			else
			{
				stat = Status::ErrorStatus;
			}
		}
		else if (stat == Status::ErrorStatus)
		{
			cout << "Error string" << endl;
			exit(0);
		}
		v.push_back(lexeme);
		ind++;
	}
	return v;
}

stack<Lexeme> PostfixNote(const vector<Lexeme> lex)
{
	stack<Lexeme>res;
	stack<Lexeme>tmp;
	for (auto l : lex)
	{
		if (l.type == LexemeType::num) 
		{
			res.push(l);
		}
		else if (l.type == LexemeType::lsc) 
		{
			tmp.push(l);
		}
		else if (l.type == LexemeType::rsc)
		{
			while (tmp.top().type != LexemeType::lsc)
			{
				res.push(tmp.top());
				tmp.pop();
			}
			tmp.pop();
		}
		else if (l.type == LexemeType::binary)
		{
			while (!tmp.empty() && tmp.top().priority > l.priority)
			{
				res.push(tmp.top());
				tmp.pop();
			}
			tmp.push(l);
		}
	}
	while (!tmp.empty())
	{
		res.push(tmp.top());
		tmp.pop();
	}
	return res;
}

double CalcPostfix(stack<Lexeme>post)
{
	return;
}

