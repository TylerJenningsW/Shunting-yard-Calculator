#include "../Calculator/Window.h"
#include "../Calculator/Factory.h"
#include "../Calculator/idList.h"
#include "pch.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalculatorTest
{
	TEST_CLASS(ButtonFactoryTests)
	{
	public:
		Window* win = nullptr;
		wxFlexGridSizer* grid = nullptr;
		wxString str = "";
		wxWindowID id = 0;
		void DoButtonCreation(size_t index) {

			win = new Window(false);
			grid = new wxFlexGridSizer(6, 4, 0, 0);
			Factory::CreateButtons(grid, win);
			str = grid->GetItem(index)->GetWindow()->GetLabel();
			id = grid->GetItem(index)->GetWindow()->GetId();
		}
		TEST_METHOD(ButtonTestTan) {
			DoButtonCreation(4);
			bool valid = false;
			if (str == "^" && id == ids::EXPONENT)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(ButtonTestCos) {
			DoButtonCreation(0.0);
			bool valid = false;
			if (str == "COS" && id == ids::COS)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(ButtonTestSin) {
			DoButtonCreation(1);
			bool valid = false;
			if (str == "SIN" && id == ids::SIN)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(ButtonTestMod) {
			DoButtonCreation(22);
			bool valid = false;
			if (str == "." && id == ids::DECI)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(ButtonTestNegative) {
			DoButtonCreation(20);
			bool valid = false;
			if (str == "+/-" && id == ids::NEGATIVE)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(ButtonTestClear) {
			DoButtonCreation(3);
			bool valid = false;
			if (str == "C" && id == ids::CLEAR)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(ButtonTestEqual) {
			DoButtonCreation(23);
			bool valid = false;
			if (str == "=" && id == ids::EQUALS)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(ButtonTestRightP) {
			DoButtonCreation(6);
			bool valid = false;
			if (str == ")" && id == ids::RIGHTP)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(ButtonTestLeftP) {
			DoButtonCreation(5);
			bool valid = false;
			if (str == "(" && id == ids::LEFTP)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
		TEST_METHOD(ButtonTestMultiply) {
			DoButtonCreation(11);
			bool valid = false;
			if (str == "*" && id == ids::MULTIPLY)
			{
				valid = true;
			}
			Assert::IsTrue(valid);
		}
	};
}
