#pragma once
#include <string>
class Token
{

public:
	enum TokenType : int {
		NUMBER = 11000,
		OPERATION,
		LPEN,
		RPEN,
		FUNCTION,
		NONE,
	};
	Token::TokenType GetTokenType();
	int GetPrecedence();
	double GetValue();
	std::string GetSymbol();
	void SetTokenType(Token::TokenType type);
	void SetPrecedence(int precedence);
	void SetValue(double val);
	void SetSymbol(std::string symbol);
	Token();
	~Token();
	Token(const Token& other);
	Token& operator=(const Token& other);

private:
	TokenType _type = NONE;
	int _precedence = 0;
	double _value = 0;
	std::string _symbol = "";
};