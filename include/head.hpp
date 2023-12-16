#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;


enum class Priority {scobe, num, plus, mult, unar};
enum LexemeType {lsc, rsc, num, binary, unary, unknown};

class Lexeme
{
public:
	Priority priority;
	LexemeType type;
	string value;
};

enum class Status {StartStatus, NumberStatus, LScobeStatus, RScobeStatus, UnaryStatus, BinaryStatus, ErrorStatus, EndStatus};