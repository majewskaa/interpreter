#include "Interpreter.h"
#include "IOrder.h"


int main() {
	IOrder task1 = IOrder("on", "LED", "now");
	IOrder task2 = IOrder("off", "LED", "now");

	Interpreter<IOrder> interpreter;

	interpreter.addOrder(task1);
	interpreter.addOrder(task2);

	return 0;
}