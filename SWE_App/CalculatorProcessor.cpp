#include "CalculatorProcessor.h"
#include "Window.h"
#include "idList.h"
#define PI 3.14159265

CalculatorProcessor* CalculatorProcessor::_processor = nullptr;


void CalculatorProcessor::ParseId(Window* parent, ids id) {
	// get pairs, the id matched to the string
	std::unordered_map<ids, std::string> btnPairs = parent->GetPairs();
	// read the current output to evaluate
	wxTextCtrl* txt = parent->GetOutput();
	std::string str = (std::string)txt->GetValue();
	std::string pressed = btnPairs.at((ids)id);
	if ((id == ids::EQUALS || id == ids::OUTPUT) && str != "") {
		_error = EvaluateExpression(str);// rpn
		_result = Calculate();// shunting
		// clean up structures
		while (!_tokenStack.empty()) {
			_tokenStack.pop();
		}
		while (!_tokenQueue.empty()) {
			_tokenQueue.pop();
		}
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
		if (str == "") {
			return;
		}
		for (unsigned int i = (str.length()-1); i >= 0; i--)	{
			char c = str[i];
			if ((std::isdigit(c) || c == ')' || c == '.') && i != 0) {
				continue;
			}
			else if (c == '-') {
				str.erase(i, 1);
				break;
			}
			else if (c == str[0] && c == '-') {
				str.erase(0, 1);
				break;

			}
			else if (c == str[0]) {
				str.insert(i, "-");
				break;

			}
			else if (std::isdigit(str[i-1])) {
				str.insert(i+1, "-");
				break;
			}
		}
		txt->Clear();
		pressed = str;
	}
	if (id == ids::CLEAR) {
		txt->Clear();
		return;
	}
	if (id == ids::BACK && str != "") {
		str.erase(str.length() - 1,1);
		txt->Clear();
		pressed = str;
	}
	else if (id == ids::BACK) {
		pressed = "";
	}
	txt->AppendText(pressed);
}

std::string CalculatorProcessor::Calculate() {
	if (_error == true)	{
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
		else if (_token1._symbol == '_') {
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
		if (_error == true) {
			return "";
		}
		_tokenStack.push(_results);
	}
	std::string str = std::to_string(_tokenStack.top()._value);
	_tokenStack.pop();
	return str;
}

bool CalculatorProcessor::EvaluateExpression(std::string strToEval) {	
	// grab expression
	unsigned int i = 0;
	unsigned int opCount = 0;
	for (i; i < strToEval.length(); ++i) {
		if (opCount > 1) {
			_error = true;
			break;
		}
		if ((std::isdigit(strToEval[i]) || strToEval[i] == '.' || strToEval[i] == '-') && strToEval[i] != strToEval.back()) {
			_currNumber += strToEval[i];
			opCount = 0;
			continue;
		}
		else if ((std::isdigit(strToEval[i]) || strToEval[i] == '.' || strToEval[i] == '-')) {
			_currNumber += strToEval[i];
			Number(_token1,_currNumber);
			_tokenQueue.push(_token1);
			_currNumber = "";
			opCount = 0;

		}
		else if(((_currNumber != "" && !(std::isdigit(strToEval[i]) || strToEval[i] == '.')) || strToEval[i] == strToEval.back()) && opCount < 1) {
			Number(_token1, _currNumber);
			_tokenQueue.push(_token1);
			_currNumber = "";
		}
		else if (!(std::isdigit(strToEval[i])) && opCount > 1) {
			_error = true;
			break;
		}
		if (strToEval[i] == '_' || strToEval[i] == '+') {
			_token1._symbol = strToEval[i];
			OperationLowest(_token1);
		}
		else if (strToEval[i] == '*' || strToEval[i] == '/' || strToEval[i] == '%') {
			_token1._symbol = strToEval[i];
			OperationHighest(_token1);
		}
		else if (strToEval[i] == '(') {
			_token1._symbol = strToEval[i];
			LeftParenthesis(_token1);
			_tokenStack.push(_token1);
		}
		else if (strToEval[i] == ')') {
			_token1._symbol = strToEval[i];
			RightParenthesis(_token1);
		}
		else if (strToEval[i] == 'S' || strToEval[i] == 'C' || strToEval[i] == 'T') {
			_token1._symbol = strToEval[i];
			Function(_token1);
			_tokenStack.push(_token1);
			i += 2;
		}
		if (_tokenStack.empty() && _token1._type == Token::RPEN) {
			_error = true;
			break;
		}
		else if (_tokenStack.empty() && _token1._type != Token::NUMBER) {
			_tokenStack.push(_token1);
			++opCount;
			continue;
		}
		if (_token1._type == Token::OPERATION
			&& _token1._precedence >= _tokenStack.top()._precedence) {
			_tokenStack.push(_token1);
			++opCount;
		}
		else if (_token1._type == Token::OPERATION && 
			_token1._precedence < _tokenStack.top()._precedence
			&& _tokenStack.top()._type == Token::OPERATION) {
			Token token = _tokenStack.top();
			_tokenStack.pop();
			_tokenStack.push(_token1);
			_tokenQueue.push(token);
			++opCount;
		}
		else if (_token1._type == Token::RPEN) {
			while (_tokenStack.top()._type != Token::LPEN && !_tokenStack.empty()) {
				Token token = _tokenStack.top();
				_tokenStack.pop();
				_tokenQueue.push(token);
			}
			if (_tokenStack.empty()) {
				_error = true;
				break;
			}
			else if (_tokenStack.top()._type == Token::LPEN)	{
				_tokenStack.pop();
			}
		}
	}
	if (_tokenStack.size() == 0)	{
		_error = true;
	}
	else {
		for (size_t i = 0; i <= _tokenStack.size(); ++i) {
			Token token = _tokenStack.top();
			_tokenStack.pop();
			_tokenQueue.push(token);
		}
	}
	return _error;
}

double CalculatorProcessor::Add(double x, double y) {
	Number(_results, std::to_string(x+y));
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
double CalculatorProcessor::MOD(double x, double y) {
	Number(_results, std::to_string(std::fmod(x,y)));
	return std::fmod(x,y);
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
	if (_error == true)	{
		return;
	}
	int count = 0;
	for (unsigned int i = _result.length() - 1; i > 0; --i)	{
		if (_result[i] != '.') {
			++count;
			continue;
		}
		_result.erase(i+1, count-2);
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
