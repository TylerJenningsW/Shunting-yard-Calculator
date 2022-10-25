#include "CalculatorProcessor.h"
#include "Window.h"
#include "idList.h"
CalculatorProcessor* CalculatorProcessor::_processor = nullptr;


void CalculatorProcessor::ParseId(Window* parent, ids id) {
	// get pairs, the id matched to the string
	std::unordered_map<ids, std::string> btnPairs = parent->GetPairs();
	// read the current output to evaluate
	wxTextCtrl* txt = parent->GetOutput();
	std::string str = (std::string)txt->GetValue();
	std::string pressed = btnPairs.at((ids)id);
	if (id == ids::EQUALS) {
		_error = EvaluateExpression(str);
		_result = Calculate();
		// clean up structures
		while (!_tokenStack.empty()) {
			_tokenStack.pop();
		}
		while (!_tokenQueue.empty()) {
			_tokenQueue.pop();
		}
		if (_error == true) {
			txt->Clear();
			txt->AppendText("ERROR!");
			return;
		}
		txt->Clear();
		txt->AppendText(_result);
		return;
	}
	if (id == ids::NEGATIVE) {
		if (str == "")
		{
			return;
		}
		bool changed = false;
		for (unsigned int i = (str.length()-1); i > 0; i--)	{
			char t = str[i];
			if ((std::isdigit(str[i]) || str[i] == ')' || str[i] == '.') && i != 0) {
				continue;
			}
			else if (str[i] == '-') {
				str.erase(i, i-1);
				changed = true;
				break;
			}
			else if (str[i] == str[0]) {
				str.insert(i+1, "-");
				changed = true;
				break;

			}
			else if (std::isdigit(str[i-1])) {
				str.insert(i+1, "-");
				changed = true;
				break;
			}
		}
		if (changed == false && str != "" && str[0] == '-') {
			str.erase(0, 1);
		}
		else if (changed == false && str != "") {
			str.insert(0, "-");
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
			BinaryFunction();
			_results._value = Add(_token3._value, _token2._value);
		}
		else if (_token1._symbol == '_') {
			BinaryFunction();
			_results._value = Subtract(_token3._value, _token2._value);
		}
		else if (_token1._symbol == '/') {
			BinaryFunction();
			_results._value = Divide(_token3._value, _token2._value);
		}
		else if (_token1._symbol == '*') {
			BinaryFunction();
			_results._value = Multiply(_token3._value, _token2._value);
		}
		else if (_token1._symbol == '%') {
			BinaryFunction();
			_results._value = MOD(_token3._value, _token2._value);
		}
		else if (_token1._symbol == "S") {
			UnaryFunction();
			_results._value = SIN(_token2._value);
		}
		else if (_token1._symbol == "C") {
			UnaryFunction();
			_results._value = COS(_token2._value);
		}
		else if (_token1._symbol == "T") {
			UnaryFunction();
			_results._value = TAN(_token2._value);
		}
		_tokenStack.push(_results);
	}
	std::string str = std::to_string(_tokenStack.top()._value);
	_tokenStack.pop();
	return str;
}

bool CalculatorProcessor::EvaluateExpression(std::string strToEval) {	
	// grab expression
	auto it = strToEval.begin();
	for (it; it != strToEval.end(); ++it)	{
		if ((std::isdigit(*it) || *it == '.' || *it == '-') && *it != strToEval.back()) {
			_currNumber += *it;
			continue;
		}
		else if ((std::isdigit(*it) || *it == '.' || *it == '-')) {
			_currNumber += *it;
			Number(_token1,_currNumber);
			_tokenQueue.push(_token1);
			_currNumber = "";
		}
		else if((_currNumber != "" && !(std::isdigit(*it) || *it == '.')) || *it == strToEval.back()) {
			Number(_token1, _currNumber);
			_tokenQueue.push(_token1);
			_currNumber = "";
		}
		if (*it == '_' || *it == '+') {
			_token1._symbol = *it;
			OperationLowest(_token1);
		}
		else if (*it == '*' || *it == '/' || *it == '%') {
			_token1._symbol = *it;
			OperationHighest(_token1);
		}
		else if (*it == '(') {
			_token1._symbol = *it;
			LeftParenthesis(_token1);
			_tokenStack.push(_token1);
		}
		else if (*it == ')') {
			_token1._symbol = *it;
			RightParenthesis(_token1);
		}
		else if (*it == 'S' || *it == 'C' || *it == 'T') {
			_token1._symbol = *it;
			Function(_token1);
			_tokenStack.push(_token1);
			it += 2;
		}
		if (_tokenStack.empty() && _token1._type == Token::RPEN) {
			_error = true;
			break;
		}
		else if (_tokenStack.empty()) {
			_tokenStack.push(_token1);
			continue;
		}
		if (_token1._type == Token::OPERATION
			&& _token1._precedence >= _tokenStack.top()._precedence) {
			_tokenStack.push(_token1);
		}
		else if (_token1._type == Token::OPERATION && 
			_token1._precedence < _tokenStack.top()._precedence) {
			Token token = _tokenStack.top();
			_tokenStack.pop();
			_tokenStack.push(_token1);
			_tokenQueue.push(token);
		}
		else if (_token1._type == Token::RPEN) {
			while (_tokenStack.top()._type != Token::LPEN || _tokenStack.empty()) {
				Token token = _tokenStack.top();
				_tokenStack.pop();
				_tokenQueue.push(token);
			}
			if (_tokenStack.top()._type == Token::LPEN)	{
				_token1 = _tokenStack.top();
				_tokenStack.pop();
			}
			else {
				_error = true;
				break;
			}
		}
	}
	for (size_t i = 0; i <= _tokenStack.size(); ++i) {
		Token token = _tokenStack.top();
		_tokenStack.pop();
		_tokenQueue.push(token);
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
	return std::sin(x);
}
double CalculatorProcessor::COS(double x) {
	Number(_results, std::to_string(std::cos(x)));
	return std::cos(x);
}
double CalculatorProcessor::TAN(double x) {
	Number(_results, std::to_string(std::tan(x)));
	return std::tan(x);
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
	token._precedence = 4;
}
void CalculatorProcessor::RightParenthesis(Token& token) {
	token._value = 0;
	token._type = Token::RPEN;
	token._precedence = 4;
}
void CalculatorProcessor::Function(Token& token) {
	token._value = 0;
	token._type = Token::FUNCTION;
	token._precedence = 4;
}
void CalculatorProcessor::BinaryFunction()
{
	_token2 = _tokenStack.top();
	_tokenStack.pop();
	_token3 = _tokenStack.top();
	_tokenStack.pop();
}
void CalculatorProcessor::UnaryFunction() {
	_token2 = _tokenStack.top();
	_tokenStack.pop();
}
CalculatorProcessor* CalculatorProcessor::GetInstance()
{
	if (_processor == nullptr) {
		_processor = new CalculatorProcessor();
	}
	return _processor;
}

void CalculatorProcessor::DestroyInstance()
{
	if (_processor != nullptr)
	{
		delete _processor;
		_processor = nullptr;
	}
}
