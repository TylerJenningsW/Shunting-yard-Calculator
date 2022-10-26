#include "../SWE_App/Window.h"
#include "../SWE_App/Factory.h"
#include "../SWE_App/idList.h"
#include "pch.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SWEAppTests
{
	TEST_CLASS(ButtonFactoryTests)
	{
	public:
		Window* win = nullptr;
		wxGridSizer* grid = nullptr;
		wxString str = "";
		wxWindowID id = 0;
		void DoButtonCreation(size_t index) {
			win = new Window();
			grid = new wxGridSizer(4, 6, 3, 3);
			Factory::CreateButtons(grid, win);
			str = grid->GetItem(index)->GetWindow()->GetLabel();
			id = grid->GetItem(index)->GetWindow()->GetId();
		}
		TEST_METHOD(ButtonTestTan) {
			DoButtonCreation(4);
			bool valid = false;
			if (str == "TAN" && id == ids::TAN)
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
			if (str == "MOD" && id == ids::MOD)
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
