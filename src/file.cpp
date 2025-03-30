#include "head.hpp"

class StringCalc
{
private:
	string str;
	Lexeme GetLexeme(const string& str, int& index, Status status) {
		string s;
		if (str[index] == '*' || str[index] == '/' || str[index] == '%')
		{
			s.push_back(str[index++]);
			return Lexeme({ LexemeType::binary, Priority::mult, s });
		}
		else if (str[index] == '+')
		{
			s.push_back(str[index++]);
			return Lexeme({ LexemeType::binary, Priority::plus, s });
		}
		else if (str[index] == '-')
		{
			if (status == Status::StartStatus || status == Status::LScobeStatus)
			{
				s.push_back(str[index++]);
				return Lexeme({ LexemeType::unary, Priority::unar, s });
			}
			else if (status == Status::NumberStatus || status == Status::RScobeStatus)
			{
				s.push_back(str[index++]);
				return Lexeme({ LexemeType::binary, Priority::plus, s });
			}
		}
		else if (str[index] == '(')
		{
			s.push_back(str[index++]);
			return Lexeme({ LexemeType::lsc, Priority::scobe, s });
		}

		else if (str[index] == '^')
		{
			s.push_back(str[index++]);
			return Lexeme({ LexemeType::binary, Priority::pow, s });
		}

		else if (str[index] == ')')
		{
			s.push_back(str[index++]);
			return Lexeme({ LexemeType::rsc, Priority::scobe, s });
		}

		else if (str.substr(index, 2) == "ln" || str.substr(index, 2) == "pi" || str.substr(index, 2) == "tg" || str.substr(index, 3) == "sin" || str.substr(index, 3) == "cos")
		{
			if (str.substr(index, 2) == "ln" || str.substr(index, 2) == "tg")
			{
				s.push_back(str[index++]);
				s.push_back(str[index++]);
				return Lexeme({ LexemeType::unary, Priority::unar, s });
			}
			else if (str.substr(index, 3) == "sin" || str.substr(index, 3) == "cos")
			{
				s.push_back(str[index++]);
				s.push_back(str[index++]);
				s.push_back(str[index++]);
				return Lexeme({ LexemeType::unary, Priority::unar, s });
			}
			else
			{
				s = to_string(std::acos(-1.0));
				index = index + 2;
				return Lexeme({ LexemeType::num, Priority::num, s });
			}
		}

		else if (str[index] == 'e') //added exponent
		{
			s = to_string(std::exp(1)); index++;
			return Lexeme({ LexemeType::num, Priority::num, s });
		}
		else if (str[index] >= '0' && str[index] <= '9')
		{
			while (str[index] >= '0' && str[index] <= '9')
			{
				s.push_back(str[index++]);
			}
			return Lexeme({ LexemeType::num, Priority::num, s });
		}
		else
		{
			return Lexeme{ LexemeType::unknown, Priority::plus, s = "" };
		}
	}

	vector<Lexeme> StringToLexeme(string str)
	{
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
					stat = Status::NumberStatus;
				}
				else if (lexeme.type == LexemeType::lsc)
				{
					stat = Status::LScobeStatus;
				}
				else if (lexeme.type == LexemeType::unary)
				{
					stat = Status::UnaryStatus;
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
				}
				else if (lexeme.type == LexemeType::diff || lexeme.type == LexemeType::num)
				{
					stat = Status::DifferentStatus;
				}
				else if (lexeme.type == LexemeType::unary)
				{
					stat = Status::UnaryStatus;
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

			//add unarnyi status
			else if (stat == Status::UnaryStatus)
			{
				if (lexeme.type == LexemeType::num)
				{
					stat = Status::NumberStatus;
				}
				else if (lexeme.type == LexemeType::lsc)
				{
					stat = Status::LScobeStatus;
				}
				else
				{
					stat = Status::ErrorStatus;
				}
			}

			else if (stat == Status::ErrorStatus)
			{

				throw "wrong expression";
			}
			v.push_back(lexeme);
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
			else if (l.type == LexemeType::lsc || l.type == LexemeType::unary) //added check for unarnyi operator lexeme 
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
				while (!tmp.empty() && tmp.top().priority >= l.priority)
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
		stack<Lexeme>pres;

		while (!res.empty())
		{
			pres.push(res.top());
			res.pop();
		}
		return pres;
	}

	double CalcPostfix(stack<Lexeme>& post)
	{
		stack<Lexeme>s;
		while (!post.empty())
		{
			Lexeme l = post.top();
			if (l.type == LexemeType::num)
			{
				s.push(l);
			}
			else if (l.type == LexemeType::binary)
			{

				double b = stod(s.top().value);
				s.pop();
				double a = stod(s.top().value);
				s.pop();
				if (l.value == "+")
				{
					s.push({ LexemeType::num, Priority::num, to_string(a + b) });
				}
				else if (l.value == "-")
				{
					s.push({ LexemeType::num, Priority::num, to_string(a - b) });
				}
				else if (l.value == "*")
				{
					s.push({ LexemeType::num, Priority::num, to_string(a * b) });
				}
				else if (l.value == "/")	//forbidden division by zero
				{
					if (b == 0)
					{
						throw "division by zero or wrong argument";
					}
					else {
						s.push({ LexemeType::num, Priority::num, to_string(a / b) });
					}
				}
				else if (l.value == "%")	//forbidden division by zero
				{
					if (b == 0)
					{
						throw "division by zero or wrong argument";
					}
					else {
						s.push({ LexemeType::num, Priority::num, to_string((long long int)a % (long long int)b) });
					}
				}
				else if (l.value == "^") //added pow
				{
					s.push({ LexemeType::num, Priority::num, to_string(pow(a, b)) });
				}
			}
			else if (l.type == LexemeType::unary)
			{
				double a = stod(s.top().value);
				s.pop();
				if (l.value == "-") //unarnyi minus
				{
					s.push({ LexemeType::num, Priority::num, to_string(-1 * a) });
				}
				else if (l.value == "ln")
				{
					if (a <= 0)
					{
						throw "division by zero or wrong argument";
					}
					s.push({ LexemeType::num, Priority::num, to_string(std::log(a)) });
				}
				else if (l.value == "tg")
				{
					s.push({ LexemeType::num, Priority::num, to_string(std::tan(a)) });
				}
				else if (l.value == "sin")
				{
					s.push({ LexemeType::num, Priority::num, to_string(std::sin(a)) });
				}
				else if (l.value == "cos")
				{
					s.push({ LexemeType::num, Priority::num, to_string(std::cos(a)) });
				}
			}

			post.pop();

		}
		double res = stod(s.top().value);
		return res;
	}

	bool MyCheckScobe(const string input)
	{
		string s = input;
		int count = 0;
		for (auto c : s) {
			if (c == '(') {
				count++;
			}
			else if (c == ')') {
				count--;
			}
			if (count < 0) return false;
		}
		return (count == 0);
	}
public:
	StringCalc(string s) : str(s) {	}
	double calculate()
	{
		double result;
		if (MyCheckScobe(str))
		{
			vector<Lexeme> lexemes = StringToLexeme(str);
			result = CalcPostfix(PostfixNote(lexemes));
		}
		else
		{
			throw "wrong scobe somewhere";
		}
		return result;

	}
};


int main()
{
	string expression;
	double result;
	cout << "Vvedite vyrazhenie bez probelov: \n";
	cin >> expression;
	cout << endl;
	StringCalc calc(expression);
	try
	{
		result = calc.calculate();
		cout << result;
	}
	catch (const char* message)
	{
		cout << message << endl;
	}
	return 0;
}
