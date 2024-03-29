#include "CalculatorProcessor.h"
#include "Window.h"
#include "idList.h"
#include <fstream>
#include <sstream>
#define PI 3.14159265

CalculatorProcessor* CalculatorProcessor::_processor = nullptr;


void CalculatorProcessor::ParseId(Window* parent, ids id) {
	// get pairs, the id matched to the string
	std::unordered_map<ids, std::string> btnPairs = parent->GetPairs();
	// read the current output to evaluate
	wxTextCtrl* txt = parent->GetOutput();
	strToEval = (std::string)txt->GetValue();
	std::string pressed = btnPairs.at((ids)id);
	if ((id == ids::EQUALS || id == ids::OUTPUT) && strToEval != "") {
		_error = EvaluateExpression();// rpn
		_result = Calculate();// shunting
		// clean up structures
		CleanUp();
		if (_error == true) {
			txt->Clear();
			txt->AppendText("0");
			_error = false;
			return;
		}
		Truncate();
		txt->Clear();
		txt->AppendText(_result);
		return;
	}
	else if (id == ids::EQUALS || id == ids::OUTPUT) {
		return;
	}
	if (id == ids::NEGATIVE) {
		if (strToEval == "") {
			return;
		}
		unsigned int n = (strToEval.length() - 1);
		for (unsigned int i = n; i >= 0; --i) {
			char c = strToEval[i];
			if ((std::isdigit(c) || c == ')' || c == '.') && i != 0) {
				continue;
			}
			else if (c == strToEval[0] && c == '-') {
				strToEval.erase(0, 1);
				break;
			}
			else if (c == strToEval[0]) {
				strToEval.insert(i, "-");
				break;
			}
			else if (c == '-' && !std::isdigit(strToEval[i - 1]) && strToEval[i - 1] != ')') {
				strToEval.erase(i, 1);
				break;
			}
			else if (std::isdigit(strToEval[i - 1])) {
				strToEval.insert(i + 1, "-");
				break;
			}
		}
		txt->Clear();
		pressed = strToEval;
	}
	if (id == ids::CLEAR) {
		txt->Clear();
		return;
	}
	if (id == ids::BACK && strToEval != "") {
		strToEval.erase(strToEval.length() - 1, 1);
		txt->Clear();
		pressed = strToEval;
	}
	else if (id == ids::BACK) {
		return;
	}
	txt->AppendText(pressed);
}

std::string CalculatorProcessor::Calculate() {
	/*

	Key:	_error: Checking for equation issues ranging from illegal chars to impossible equations
			_token(1-3): Used to pass to the shunting yard algorithm
			_tokenStack: Used to temporarily hold an infix notation's tokens
			_tokenQueue: The postfix notation equation's tokens
			_results: The end output token of the given equation

	*/
	if (_error == true) {
		return "";
	}
	while (!_tokenQueue.empty()) {
		_token1 = _tokenQueue.front();
		_tokenQueue.pop();
		if (_token1.GetTokenType() == Token::LPEN || _token2.GetTokenType() == Token::RPEN) {
			_error = true;
			break;
		}
		else if (_token1.GetTokenType() == Token::NUMBER) {
			_tokenStack.push(_token1);
			continue;
		}
		else if (_token1.GetSymbol() == '+') {
			_error = BinaryFunction();
			_results.SetValue(Add(_token3.GetValue(), _token2.GetValue()));
		}
		else if (_token1.GetSymbol() == '-') {
			_error = BinaryFunction();
			_results.SetValue(Subtract(_token3.GetValue(), _token2.GetValue()));
		}
		else if (_token1.GetSymbol() == '/') {
			_error = BinaryFunction();
			_results.SetValue(Divide(_token3.GetValue(), _token2.GetValue()));
		}
		else if (_token1.GetSymbol() == '*') {
			_error = BinaryFunction();
			_results.SetValue(Multiply(_token3.GetValue(), _token2.GetValue()));
		}
		else if (_token1.GetSymbol() == '%') {
			_error = BinaryFunction();
			_results.SetValue(MOD(_token3.GetValue(), _token2.GetValue()));
		}
		else if (_token1.GetSymbol() == '^') {
			_error = BinaryFunction();
			_results.SetValue(EXPONENT(_token3.GetValue(), _token2.GetValue()));
		}
		else if (_token1.GetSymbol() == "S") {
			_error = UnaryFunction();
			_results.SetValue(SIN(_token2.GetValue()));
		}
		else if (_token1.GetSymbol() == "C") {
			_error = UnaryFunction();
			_results.SetValue(COS(_token2.GetValue()));
		}
		else if (_token1.GetSymbol() == "T") {
			_error = UnaryFunction();
			_results.SetValue(TAN(_token2.GetValue()));
		}
		if (_error == true && _tokenStack.size() == 1) {
			_error = false;
			break;
		}
		else if (_error == false) {
			_tokenStack.push(_results);
		}
		else if (_error == true) {
			return "";
		}
	}
	std::string str = std::to_string(_tokenStack.top().GetValue());
	_tokenStack.pop();
	return str;
}

void CalculatorProcessor::CleanUp() {
	while (!_tokenStack.empty()) {
		_tokenStack.pop();
	}
	while (!_tokenQueue.empty()) {
		_tokenQueue.pop();
	}
}

bool CalculatorProcessor::EvaluateExpression() {
	/*

	Key:	_error: Checking for equation issues ranging from illegal chars to impossible equations
			_token(1-3): Used to pass to the shunting yard algorithm
			_tokenStack: Used to temporarily hold an infix notation's tokens
			_tokenQueue: The postfix notation equation's tokens
			_results: The end output token of the given equation
	*/
	if (strToEval.length() > 256) {
		_error = true;
		return _error;
	}
	// check if multiplication is implied by parentheses
	strToEval = MultiplyParentheses();
	// grab expression
	unsigned int i = 0;
	unsigned int opCount = 0;
	bool isOperator = false;
	int decimalCount = 0;
	int negativeCount = 0;
	for (i; i < strToEval.length(); ++i) {
		std::string symbol = "";
		char c = strToEval[i];
		isOperator = false;
		// determine if char is op or negative
		if (strToEval[i] == '-' && i != 0 && strToEval[i - 1] != '-' && strToEval[i - 1] != '(') {
			isOperator = true;
		}
		else if (isOperator != true && strToEval[i] == '-') {
			++negativeCount;
		}
		if (strToEval[i] == '.') {
			++decimalCount;
		}
		// if user were to spam symbols that couldn't be calculated
		if (decimalCount > 1 || opCount > 1 || negativeCount > 1) {
			_error = true;
			break;
		}
		// track the number string to be tokenized
		if ((std::isdigit(strToEval[i]) || strToEval[i] == '.' || strToEval[i] == '-')// number cases
			&& i != (strToEval.length() - 1)// not the end of the string
			&& isOperator == false)// and isn't an operation
		{
			_currNumber += strToEval[i];
			opCount = 0;
			continue;
		}
		// tokenize number if at the end of the equation
		else if (std::isdigit(strToEval[i]) && i == (strToEval.length() - 1)) {
			_currNumber += strToEval[i];
			Number(_token1, _currNumber);
			_tokenQueue.push(_token1);
			_currNumber = "";
			opCount = 0;
			decimalCount = 0;
			negativeCount = 0;
		}
		// tokenize number if at the end of the number
		// i.e not a digit, or expression is over
		else if ((_currNumber != "" && !(std::isdigit(strToEval[i])) || (_currNumber != "" && i == (strToEval.length() - 1)
			&& opCount < 1))) {
			_error = Number(_token1, _currNumber);
			_tokenQueue.push(_token1);
			_currNumber = "";
			decimalCount = 0;
			negativeCount = 0;
		}
		else if (!(std::isdigit(strToEval[i])) && opCount > 1) {
			_error = true;
			break;
		}
		// tokenize operators
		if (strToEval[i] == '-' && isOperator == true) {
			_token1.SetSymbol(std::string(1, strToEval[i]));
			OperationLowest(_token1);
		}
		else if (strToEval[i] == '+') {
			_token1.SetSymbol(std::string(1, strToEval[i]));
			OperationLowest(_token1);
		}
		else if (strToEval[i] == '*' || strToEval[i] == '/' || strToEval[i] == '%') {
			_token1.SetSymbol(std::string(1, strToEval[i]));
			OperationHigh(_token1);
		}
		else if (strToEval[i] == '^') {
			_token1.SetSymbol(std::string(1, strToEval[i]));
			OperationExponent(_token1);
		}
		// tokenize parentheses
		else if (strToEval[i] == '(') {
			_token1.SetSymbol(std::string(1, strToEval[i]));
			LeftParenthesis(_token1);
			_tokenStack.push(_token1);
		}
		else if (strToEval[i] == ')') {
			_token1.SetSymbol(std::string(1, strToEval[i]));
			RightParenthesis(_token1);
		}
		// tokenize functions
		else if (strToEval[i] == 'S' || strToEval[i] == 'C' || strToEval[i] == 'T') {
			_token1.SetSymbol(std::string(1, strToEval[i]));
			Function(_token1);
			_tokenStack.push(_token1);
			i += 2;
		}
		// parenthesis mismatch case
		if ((_tokenStack.empty() && _token1.GetTokenType() == Token::RPEN)) {
			_error = true;
			break;
		}
		// no need for comparison if the stack is empty
		else if (_tokenStack.empty() && _token1.GetTokenType() != Token::NUMBER && _token1.GetTokenType() != Token::NONE) {
			_tokenStack.push(_token1);
			++opCount;
			continue;
		}
		if (_token1.GetTokenType() == Token::OPERATION
			&& _token1.GetPrecedence() >= _tokenStack.top().GetPrecedence()) {
			_tokenStack.push(_token1);
			++opCount;
		}
		// swap high precedence operator onto the queue
		else if (_token1.GetTokenType() == Token::OPERATION &&
			_token1.GetPrecedence() < _tokenStack.top().GetPrecedence()
			&& _tokenStack.top().GetTokenType() == Token::OPERATION) {
			Token token = _tokenStack.top();
			_tokenStack.pop();
			_tokenStack.push(_token1);
			_tokenQueue.push(token);
			++opCount;
		}
		// pop everything within the parentheses into the queue
		else if (_token1.GetTokenType() == Token::RPEN) {
			while (_tokenStack.top().GetTokenType() != Token::LPEN && !_tokenStack.empty()) {
				Token token = _tokenStack.top();
				_tokenStack.pop();
				_tokenQueue.push(token);
			}
			// missing leftP case
			if (_tokenStack.empty()) {
				_error = true;
				break;
			}
			else if (_tokenStack.top().GetTokenType() == Token::LPEN) {
				_tokenStack.pop();
			}
		}
	}
	// no operators case
	if (_tokenStack.size() == 0 || _tokenQueue.size() == 0) {
		_error = true;
	}
	// push everything into the final reverse polish notation queue
	else {
		while (!_tokenStack.empty()) {
			Token token = _tokenStack.top();
			_tokenStack.pop();
			_tokenQueue.push(token);
		}
	}
	// return whether or not there was an error found
	return _error;
}

double CalculatorProcessor::Add(double x, double y) {
	_error = Number(_results, std::to_string(x + y));
	return x + y;
}
double CalculatorProcessor::Subtract(double x, double y) {
	_error = Number(_results, std::to_string(x - y));
	return x - y;
}
double CalculatorProcessor::Divide(double x, double y) {
	_error = Number(_results, std::to_string(x / y));
	return x / y;
}
double CalculatorProcessor::Multiply(double x, double y) {
	_error = Number(_results, std::to_string(x * y));
	return x * y;
}
std::string CalculatorProcessor::MultiplyParentheses() {
	for (int i = 0; i < strToEval.length(); ++i) {
		if (strToEval[i] != '(' && strToEval[i] != ')') {
			continue;
		}
		else if (strToEval[i] == '(' && i != 0 && std::isdigit(strToEval[i - 1])) {
			strToEval.insert(i, 1, '*');
		}
		else if (strToEval[i] == ')' && i < (strToEval.length() - 1) && (std::isdigit(strToEval[i + 1]) || strToEval[i + 1] == '(')) {
			strToEval.insert(i + 1, 1, '*');
		}
	}
	return strToEval;
}
double CalculatorProcessor::MOD(double x, double y) {
	_error = Number(_results, std::to_string(std::fmod(x, y)));
	return std::fmod(x, y);
}
double CalculatorProcessor::EXPONENT(double x, double y) {
	_error = Number(_results, std::to_string(std::pow(x, y)));
	return std::pow(x,y);
}
double CalculatorProcessor::SIN(double x) {
	_error = Number(_results, std::to_string(std::sin(x)));
	return std::sin(x * PI / 180.0);
}
double CalculatorProcessor::COS(double x) {
	_error = Number(_results, std::to_string(std::cos(x)));
	return std::cos(x * PI / 180.0);
}
double CalculatorProcessor::TAN(double x) {
	_error = Number(_results, std::to_string(std::tan(x)));
	return std::tan(x * PI / 180.0);
}
bool CalculatorProcessor::Number(Token& token, std::string str) {
	if (str != '.' && str != '-') {
		token.SetSymbol(str);
		token.SetValue(std::stod(str));
		token.SetTokenType(Token::NUMBER);
		token.SetPrecedence(0);
		return false;
	}
	return true;
}

void CalculatorProcessor::OperationLowest(Token& token) {
	token.SetValue(0);
	token.SetTokenType(Token::OPERATION);
	token.SetPrecedence(2);
}

void CalculatorProcessor::OperationHigh(Token& token) {
	token.SetValue(0);
	token.SetTokenType(Token::OPERATION);
	token.SetPrecedence(3);
}

void CalculatorProcessor::OperationExponent(Token& token) {
	token.SetValue(0);
	token.SetTokenType(Token::OPERATION);
	token.SetPrecedence(4);
}

void CalculatorProcessor::LeftParenthesis(Token& token) {
	token.SetValue(0);
	token.SetTokenType(Token::LPEN);
	token.SetPrecedence(-1);
}
void CalculatorProcessor::RightParenthesis(Token& token) {
	token.SetValue(0);
	token.SetTokenType(Token::RPEN);
	token.SetPrecedence(-1);
}
void CalculatorProcessor::Function(Token& token) {
	token.SetValue(0);
	token.SetTokenType(Token::FUNCTION);
	token.SetPrecedence(4);
}
bool CalculatorProcessor::BinaryFunction() {
	if (_tokenStack.size() >= 2) {
		_token2 = _tokenStack.top();
		_tokenStack.pop();
		_token3 = _tokenStack.top();
		_tokenStack.pop();
		return false;
	}
	return true;
}
bool CalculatorProcessor::UnaryFunction() {
	if (_tokenStack.size() >= 1) {
		_token2 = _tokenStack.top();
		_tokenStack.pop();
		return false;
	}
	return true;
}
void CalculatorProcessor::Truncate() {
	if (_error == true) {
		return;
	}

	std::stringstream ss; //for parsing int
	std::string decimalPlacesStr = "";
	std::fstream config;
	int decimalPlaces = 0;
	config.open("config.txt", std::ios::in);
	if (config.is_open()) {
		std::getline(config, decimalPlacesStr);
		config.close();
	}
	ss << decimalPlacesStr;
	ss >> decimalPlaces;
	int countPlaces = std::stoi(_result);
	std::string toBeCounted = std::to_string(countPlaces);
	int lengthBeforeDecimal = toBeCounted.length();
	if (decimalPlaces > _result.length()) { decimalPlaces = 3; }
	_result.erase(lengthBeforeDecimal+1, decimalPlaces);
}
CalculatorProcessor* CalculatorProcessor::GetInstance() {
	if (_processor == nullptr) {
		_processor = new CalculatorProcessor();
	}
	return _processor;
}

void CalculatorProcessor::DestroyInstance() {
	if (_processor != nullptr)
	{
		delete _processor;
		_processor = nullptr;
	}
}
