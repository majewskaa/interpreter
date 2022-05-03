#include "../Interpreter/IOrder.h"
#include "../Interpreter/IOrder.cpp"
#include "CppUnitTest.h"
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testIOrder
{
	TEST_CLASS(testIOrder)
	{
	public:

		TEST_METHOD(Simple)
		{
			IOrder task = IOrder("on", "LED", "2017-01-27 06:35:12");
			Assert::AreEqual(std::string("LED"), task.getDevName());
			Assert::AreEqual(std::string("on"), task.getAction());
			Assert::AreEqual(std::string("2017-01-27 06:35:12"), task.getTextTime());
		}

		TEST_METHOD(NowTime)
		{
			IOrder task = IOrder("on", "LED", "NOW");
			Assert::IsTrue(timeValid(task.getTextTime()));
		}

	};
}
