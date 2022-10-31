#include "Token.h"
#include "idList.h"
#include <string>

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
