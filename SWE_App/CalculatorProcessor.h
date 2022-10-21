#pragma once
#include <string>
#include <stack>
#include <queue>
#include "Token.h"
#include "idList.h"
class Window;
class CalculatorProcessor
{
private:
	static CalculatorProcessor* _processor;
	CalculatorProcessor() {}
	std::string _currNumber = "";
	std::string _result = "";
	bool _error = false;
	Token _token1;
	Token _token2;
	Token _token3;
	Token _results;
	std::stack<Token> _tokenStack;
	std::queue<Token> _tokenQueue;
public:
	CalculatorProcessor(CalculatorProcessor& other) = delete;
	CalculatorProcessor& operator=(CalculatorProcessor& other) = delete;
	void ParseId(Window* parent, ids id);
	std::string Calculate();
	std::string GetCurrNumber();
	bool EvaluateExpression(std::string strToEval);
	void SetCurrNumber(std::string str);
	double Add(double x, double y);
	double Subtract(double x, double y);
	double Divide(double x, double y);
	double Multiply(double x, double y);
	double MOD(double x, double y);
	double SIN(double x);
	double COS(double x);
	double TAN(double x);
	void Number(Token& token, std::string str);
	void OperationLowest(Token& token);
	void OperationHighest(Token& token);
	void LeftParenthesis(Token& token);
	void RightParenthesis(Token& token);
	void Function(Token& token);
	void BinaryFunction();
	void UnaryFunction();
	static CalculatorProcessor* GetInstance();
	static void DestroyInstance();
};
