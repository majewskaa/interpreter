#include "IOrder.h"
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <ctime>
#include <iomanip>


std::string chronoToString(t_point p_time) {
	std::time_t time = std::chrono::system_clock::to_time_t(p_time);	// chrono -> string
	std::tm* ptm = std::localtime(&time);
	char tBuffer[32];
	std::strftime(tBuffer, 32, DATE_FORMAT, ptm);

	return tBuffer;
}


t_point stringToChrono(const std::string textTime) {
	std::tm tm = {};
	std::stringstream ss(textTime);
	ss >> std::get_time(&tm, DATE_FORMAT);
	t_point tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

	return tp;
}

bool timeValid(std::string timeString) {
	if (timeString.size() != 19) return false;

	if (timeString[4] != '-' || timeString[7] != '-' || timeString[10] != ' ' || timeString[13] != ':' || timeString[16] != ':') return false;

	for (int it = 0; it != timeString.size(); ++it) {
		if (it == 4 || it == 7 || it == 10 || it == 13 || it == 16) continue;
		if (!isdigit(timeString[it])) return false;
	}
	return true;
}


void IOrder::setTime(const std::string newTime) {
	// checking if newTime == "now"
	std::string t = newTime;
	std::for_each(t.begin(), t.end(), [](char& c) {c = ::tolower(c); });

	if (t == "now") {

		t_point nowChrono = std::chrono::system_clock::now();	// getting "chrono now"
		IOrder::setTime(nowChrono);
		return;
	}

	if (timeValid(newTime)) {
		IOrder::textTime = newTime;
		IOrder::time = stringToChrono(newTime);
	}

	else	std::cout << "Invalid data";
}

void IOrder::setTime(const t_point newTime) {
	IOrder::time = newTime;
	IOrder::textTime = chronoToString(newTime);
}

std::string IOrder::info() {
	std::string info = "";
	info.append(getAction());
	if (getDevName() != "0") {
		info.append("\t");
		info.append(getDevName());
	}
	if (getTextTime() != "0") {
		info.append("\t");
		info.append(getTextTime());
	}
	return info;
}


std::ostream& operator<<(std::ostream& os, IOrder& order)
{
	os << order.info();
	return os;
}


std::istream& operator>>(std::istream& input, IOrder& order)
{
	std::vector <std::string> data;
	std::string temp;
	while (getline(input, temp, '\t')) {
		data.push_back(temp);
	}

	int dSize = data.size();

	if (dSize < 2 || dSize > 3) {
		std::cout << "Invalid data";
		return input;
	}

	order.deviceName = data.at(0);
	order.action = data.at(1);

	if (dSize == 3)	order.setTime(data.at(1));

	return input;
}


bool operator==(const IOrder& lhs, const IOrder& rhs) {
	if (lhs.action == rhs.action && lhs.deviceName == rhs.deviceName && lhs.time == rhs.time)
		return true;
	return false;
}


Move::Move(const std::string devName, const int n_x, const int n_y, const std::string n_time) {
	this->deviceName = devName;
	this->action = "move";
	this->setPosition(n_x, n_y);
	this->setTime(n_time);
}
