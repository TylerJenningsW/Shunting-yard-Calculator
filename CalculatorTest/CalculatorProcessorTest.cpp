#include "../Calculator/Window.h"
#include "../Calculator/CalculatorProcessor.h"
#include "pch.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SWEAppTests
{
	TEST_CLASS(CalculatorProcessorTest)
	{
	public:
		TEST_METHOD(TwoPlusTwoIsFour) {
			CalculatorProcessor* calc = CalculatorProcessor::GetInstance();
			double result1 = calc->Add(2, 2);
			Assert::AreEqual(4.0, result1);
		}
		TEST_METHOD(TwoMinusTwoIsZero) {
			CalculatorProcessor* calc = CalculatorProcessor::GetInstance();
			double result2 = calc->Subtract(2, 2);
			Assert::AreEqual(0.0, result2);
		}
		TEST_METHOD(TwoTimesTwoIsFour) {
			CalculatorProcessor* calc = CalculatorProcessor::GetInstance();
			double result3 = calc->Multiply(2, 2);
			Assert::AreEqual(4.0, result3);
		}
		TEST_METHOD(TwoDividedByTwoIsOne) {
			CalculatorProcessor* calc = CalculatorProcessor::GetInstance();
			double result4 = calc->Divide(2, 2);
			Assert::AreEqual(1.0, result4);
		}
		TEST_METHOD(TwoModTwoIsZero) {
			CalculatorProcessor* calc = CalculatorProcessor::GetInstance();
			double result5 = calc->MOD(2, 2);
			Assert::AreEqual(0.0, result5);
		}
		TEST_METHOD(SinOfOne) {
			CalculatorProcessor* calc = CalculatorProcessor::GetInstance();
			double result6 = calc->SIN(1.0);
			bool valid = false;
			if (result6 > 0.017 && result6 < 0.019)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(CosOfOne) {
			CalculatorProcessor* calc = CalculatorProcessor::GetInstance();
			double result7 = calc->COS(1.0);
			bool valid = false;
			if (result7 > 0.98 && result7 < 1.01)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(TanOfOne) {
			CalculatorProcessor* calc = CalculatorProcessor::GetInstance();
			double result8 = calc->TAN(1.0);
			bool valid = false;
			if (result8 > 0.017 && result8 < 0.019)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(TokenPrecedenceIsTwo) {
			CalculatorProcessor* calc = CalculatorProcessor::GetInstance();
			Token token;
			calc->OperationLowest(token);
			bool valid = false;
			if (token._precedence == 2)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(testhighest) {
			CalculatorProcessor* calc = CalculatorProcessor::GetInstance();
			Token token;
			calc->OperationHighest(token);
			bool valid = false;
			if (token._precedence == 3)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
	};
}
