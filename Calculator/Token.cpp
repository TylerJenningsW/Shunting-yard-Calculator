#include "Token.h"
#include "idList.h"
#include <string>

Token::TokenType Token::GetTokenType()
{
	return _type;
}

int Token::GetPrecedence()
{
	return _precedence;
}

double Token::GetValue()
{
	return _value;
}

std::string Token::GetSymbol()
{
	return _symbol;
}

void Token::SetTokenType(Token::TokenType type)
{
	_type = type;
}

void Token::SetPrecedence(int precedence)
{
	_precedence = precedence;
}

void Token::SetValue(double val)
{
	_value = val;
}

void Token::SetSymbol(std::string symbol)
{
	_symbol = symbol;
}

Token::Token() {

}
Token::~Token() {
}

Token::Token(const Token& other)
{
	_symbol = other._symbol;
	_value = other._value;
	_precedence = other._precedence;
	_type = other._type;
}

Token& Token::operator=(const Token& other)
{
	if (this != &other)
	{
		_symbol = other._symbol;
		_value = other._value;
		_precedence = other._precedence;
		_type = other._type;
	}
	return *this;
}
