#pragma once
#include <unordered_map>
class Token
{
private:

public:
	enum TokenType : int {
		NUMBER = 11000,
		OPERATION,
		LPEN,
		RPEN,
		FUNCTION,
		NONE,
	};
	TokenType _type = NONE;
	int _precedence = 0;
	double _value = 0;
	std::string _symbol = "";
	Token();
	~Token();
	Token(const Token& other);
	Token& operator=(const Token& other);
};