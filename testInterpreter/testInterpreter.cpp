#include "../Interpreter/Interpreter.h"
#include "CppUnitTest.h"
#include "../Interpreter/IOrder.h"
#include "../Interpreter/IOrder.cpp"
#include "../Interpreter/Interpreter.h"
#include "../Interpreter/Interpreter.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testInterpreter
{
	TEST_CLASS(testInterpreter)
	{
	public:
		
		TEST_METHOD(Iterator)
		{
			IOrder task1 = IOrder("on", "LED", "now");
			IOrder task2 = IOrder("off", "LED", "now");
			IOrder task3 = IOrder("write", "pen", "now");
			IOrder task4 = IOrder("water", "plant", "now");
			IOrder task5 = IOrder("pick_up", "letter", "now");

			IOrder o_list[] = { task1, task2, task3, task4, task5 };

			Interpreter<IOrder> interpreter;
			interpreter.addOrder(task1);
			interpreter.addOrder(task2);
			interpreter.addOrder(task3);
			interpreter.addOrder(task4);
			interpreter.addOrder(task5);

			int it = 0;

			for (IOrder ord : interpreter) {
				Assert::IsTrue(o_list[it] == ord);
				it++;
			}
		}

		TEST_METHOD(DeviceIterator)
		{
			IOrder task1 = IOrder("on", "LED", "now");
			IOrder task2 = IOrder("off", "LED", "now");
			IOrder task3 = IOrder("write", "pen", "now");
			IOrder task4 = IOrder("water", "plant", "now");
			IOrder task5 = IOrder("pick_up", "LED", "now");

			IOrder o_list[] = { task1, task2, task5 };

			Interpreter<IOrder> interpreter;
			interpreter.addOrder(task1);
			interpreter.addOrder(task2);
			interpreter.addOrder(task3);
			interpreter.addOrder(task4);
			interpreter.addOrder(task5);

			int list_it = 0;

			for (Interpreter<IOrder>::DeviceIterator it = interpreter.d_begin("LED"); it != interpreter.d_end("LED"); ++it) {
				Assert::IsTrue(o_list[list_it] == *it);
				list_it++;
			}
		}
		

		TEST_METHOD(IntervalIterator)
		{
			IOrder task1 = IOrder("on", "LED", "2020-11-27 06:35:12");
			IOrder task2 = IOrder("off", "LED", "2021-01-27 06:35:12");
			IOrder task3 = IOrder("write", "pen", "2020-12-27 06:35:12");
			IOrder task4 = IOrder("water", "plant", "2020-10-27 06:35:12");
			IOrder task5 = IOrder("pick_up", "LED", "2021-02-27 06:35:12");

			IOrder o_list[] = { task1, task3, task4 };

			Interpreter<IOrder> interpreter;
			interpreter.addOrder(task1);
			interpreter.addOrder(task2);
			interpreter.addOrder(task3);
			interpreter.addOrder(task4);
			interpreter.addOrder(task5);

			t_point start = stringToChrono("2020-10-26 06:35:12");
			t_point stop = stringToChrono("2021-01-01 06:35:12");

			int list_it = 0;

			for (Interpreter<IOrder>::IntervalIterator it = interpreter.i_begin(start, stop); it != interpreter.i_end(start, stop); ++it) {
				Assert::IsTrue(o_list[list_it] == *it);
				list_it++;
			}
		}
	};
}
