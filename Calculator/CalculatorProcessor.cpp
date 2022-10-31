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
		unsigned int n = (strToEval.length() - 2);
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
	if (_error == true) {
		return "";
	}
	while (!_tokenQueue.empty()) {
		_token1 = _tokenQueue.front();
		_tokenQueue.pop();
		if (_token1._type == Token::LPEN || _token2._type == Token::RPEN) {
			_error = true;
			break;
		}
		else if (_token1._type == Token::NUMBER) {
			_tokenStack.push(_token1);
			continue;
		}
		else if (_token1._symbol == '+') {
			_error = BinaryFunction();
			_results._value = Add(_token3._value, _token2._value);
		}
		else if (_token1._symbol == '-') {
			_error = BinaryFunction();
			_results._value = Subtract(_token3._value, _token2._value);
		}
		else if (_token1._symbol == '/') {
			_error = BinaryFunction();
			_results._value = Divide(_token3._value, _token2._value);
		}
		else if (_token1._symbol == '*') {
			_error = BinaryFunction();
			_results._value = Multiply(_token3._value, _token2._value);
		}
		else if (_token1._symbol == '%') {
			_error = BinaryFunction();
			_results._value = MOD(_token3._value, _token2._value);
		}
		else if (_token1._symbol == "S") {
			_error = UnaryFunction();
			_results._value = SIN(_token2._value);
		}
		else if (_token1._symbol == "C") {
			_error = UnaryFunction();
			_results._value = COS(_token2._value);
		}
		else if (_token1._symbol == "T") {
			_error = UnaryFunction();
			_results._value = TAN(_token2._value);
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
	std::string str = std::to_string(_tokenStack.top()._value);
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
	// check if multiplication is implied by parentheses
	strToEval = MultiplyParentheses();
	// grab expression
	unsigned int i = 0;
	unsigned int opCount = 0;
	bool isOperator = false;
	int decimalCount = 0;
	for (i; i < strToEval.length(); ++i) {
		char c = strToEval[i];
		isOperator = false;
		// if user were to spam symbols that couldn't be calculated
		if (opCount > 1) {
			_error = true;
			break;
		}
		if (strToEval[i] == '.') {
			++decimalCount;
		}
		if (decimalCount > 1) {
			_error = true;
			break;
		}
		// determine if char is op or negative
		if (strToEval[i] == '-' && i != 0 && strToEval[i - 1] != '-' && strToEval[i - 1] != '(') {
			isOperator = true;
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
		}
		// tokenize number if at the end of the number
		// i.e not a digit, or expression is over
		else if (((_currNumber != "" && !(std::isdigit(strToEval[i]))) || i == (strToEval.length() - 1)
			&& opCount < 1)) {
			Number(_token1, _currNumber);
			_tokenQueue.push(_token1);
			_currNumber = "";
			decimalCount = 0;
		}
		else if (!(std::isdigit(strToEval[i])) && opCount > 1) {
			_error = true;
			break;
		}
		// tokenize operators
		if (strToEval[i] == '-' && isOperator == true) {
			_token1._symbol = strToEval[i];
			OperationLowest(_token1);
		}
		else if (strToEval[i] == '+') {
			_token1._symbol = strToEval[i];
			OperationLowest(_token1);
		}
		else if (strToEval[i] == '*' || strToEval[i] == '/' || strToEval[i] == '%') {
			_token1._symbol = strToEval[i];
			OperationHighest(_token1);
		}
		// tokenize parentheses
		else if (strToEval[i] == '(') {
			_token1._symbol = strToEval[i];
			LeftParenthesis(_token1);
			_tokenStack.push(_token1);
		}
		else if (strToEval[i] == ')') {
			_token1._symbol = strToEval[i];
			RightParenthesis(_token1);
		}
		// tokenize functions
		else if (strToEval[i] == 'S' || strToEval[i] == 'C' || strToEval[i] == 'T') {
			_token1._symbol = strToEval[i];
			Function(_token1);
			_tokenStack.push(_token1);
			i += 2;
		}
		// parenthesis mismatch case
		if (_tokenStack.empty() && _token1._type == Token::RPEN) {
			_error = true;
			break;
		}
		// no need for comparison if the stack is empty
		else if (_tokenStack.empty() && _token1._type != Token::NUMBER && _token1._type != Token::NONE) {
			_tokenStack.push(_token1);
			++opCount;
			continue;
		}
		if (_token1._type == Token::OPERATION
			&& _token1._precedence >= _tokenStack.top()._precedence) {
			_tokenStack.push(_token1);
			++opCount;
		}
		// swap high precedence operator onto the queue
		else if (_token1._type == Token::OPERATION &&
			_token1._precedence < _tokenStack.top()._precedence
			&& _tokenStack.top()._type == Token::OPERATION) {
			Token token = _tokenStack.top();
			_tokenStack.pop();
			_tokenStack.push(_token1);
			_tokenQueue.push(token);
			++opCount;
		}
		// pop everything within the parentheses into the queue
		else if (_token1._type == Token::RPEN) {
			while (_tokenStack.top()._type != Token::LPEN && !_tokenStack.empty()) {
				Token token = _tokenStack.top();
				_tokenStack.pop();
				_tokenQueue.push(token);
			}
			// missing leftP case
			if (_tokenStack.empty()) {
				_error = true;
				break;
			}
			else if (_tokenStack.top()._type == Token::LPEN) {
				_tokenStack.pop();
			}
		}
	}
	// no operators case
	if (_tokenStack.size() == 0) {
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
	Number(_results, std::to_string(x + y));
	return x + y;
}
double CalculatorProcessor::Subtract(double x, double y) {
	Number(_results, std::to_string(x - y));
	return x - y;
}
double CalculatorProcessor::Divide(double x, double y) {
	Number(_results, std::to_string(x / y));
	return x / y;
}
double CalculatorProcessor::Multiply(double x, double y) {
	Number(_results, std::to_string(x * y));
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
	Number(_results, std::to_string(std::fmod(x, y)));
	return std::fmod(x, y);
}
double CalculatorProcessor::SIN(double x) {
	Number(_results, std::to_string(std::sin(x)));
	return std::sin(x * PI / 180.0);
}
double CalculatorProcessor::COS(double x) {
	Number(_results, std::to_string(std::cos(x)));
	return std::cos(x * PI / 180.0);
}
double CalculatorProcessor::TAN(double x) {
	Number(_results, std::to_string(std::tan(x)));
	return std::tan(x * PI / 180.0);
}
void CalculatorProcessor::Number(Token& token, std::string str) {
	token._symbol = str;
	token._value = std::stod(str);
	token._type = Token::NUMBER;
	token._precedence = 0;
}

void CalculatorProcessor::OperationLowest(Token& token) {
	token._value = 0;
	token._type = Token::OPERATION;
	token._precedence = 2;
}

void CalculatorProcessor::OperationHighest(Token& token) {
	token._value = 0;
	token._type = Token::OPERATION;
	token._precedence = 3;
}

void CalculatorProcessor::LeftParenthesis(Token& token) {
	token._value = 0;
	token._type = Token::LPEN;
	token._precedence = -1;
}
void CalculatorProcessor::RightParenthesis(Token& token) {
	token._value = 0;
	token._type = Token::RPEN;
	token._precedence = -1;
}
void CalculatorProcessor::Function(Token& token) {
	token._value = 0;
	token._type = Token::FUNCTION;
	token._precedence = 4;
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
	int count = 0;
	int decimalPlaces = 0;
	config.open("config.txt", std::ios::in);
	if (config.is_open()) {
		std::getline(config, decimalPlacesStr);
		config.close();
	}
	ss << decimalPlacesStr;
	ss >> decimalPlaces;
	for (unsigned int i = _result.length() - 1; i > 0; --i) {
		if (_result[i] != '.') {
			++count;
			continue;
		}
		_result.erase(i + decimalPlaces + 1, count - decimalPlaces);
	}
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
