#pragma once

#include <iostream>
#include <chrono>
#include <iterator>
#include <cstddef>
#include <algorithm>

using t_point = std::chrono::time_point<std::chrono::system_clock>;

const char DATE_FORMAT[] = "%Y-%m-%d %H:%M:%S";	// if changed timeValid() must change

class IOrder {
private:

	t_point time;
	std::string textTime;

public:
	IOrder() {};

	IOrder(const std::string action, const std::string devName, const t_point time) : deviceName(devName), action(action) {
		IOrder::setTime(time);
	};

	IOrder(const std::string action, const std::string devName, const std::string time) : deviceName(devName), action(action) {
		IOrder::setTime(time);
	};

	std::string deviceName;
	std::string action;

	void setTime(const std::string newTime);
	void setTime(const t_point newTime);

	std::string getDevName() { return this->deviceName; }
	std::string getAction() { return this->action; }
	t_point getTime() { return this->time; }
	std::string getTextTime() { return this->textTime; }

	std::string info();

	friend bool operator==(const IOrder& lhs, const IOrder& rhs);
	friend std::istream& operator>>(std::istream& input, IOrder& order);

};


class Move : virtual IOrder {
private:
	int x;
	int y;
public:
	Move(const std::string devName = "0", const int n_x = 0, const int n_y = 0, const std::string n_time = "0");
	void setPosition(int n_x, int n_y) { this->x = n_x; this->y = n_y; }
};
