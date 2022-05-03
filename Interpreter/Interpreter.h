#pragma once
#include "IOrder.h"
#include <iostream>
#include <vector>
#include <utility>
#include <chrono>


template <typename Interpreter>
class InterIterator {
public:
	using ValueType = typename Interpreter::ValueType;
	using PointerType = ValueType*;
	using ReferenceType = ValueType&;

	InterIterator(PointerType ptr) : o_ptr(ptr) {};

	InterIterator& operator++() {
		o_ptr++;
		return *this;
	}

	InterIterator operator++(int) {
		InterIterator iterator = *this;
		o_ptr++;
		return iterator;
	}

	InterIterator& operator--() {
		o_ptr--;
		return *this;
	}

	InterIterator operator--(int) {
		InterIterator iterator = *this;
		--(this);
		return iterator;
	}

	ReferenceType operator[](int index) {
		return *(o_ptr + index);
	}

	PointerType operator->() {
		return o_ptr;
	}

	ReferenceType operator*() {
		return *o_ptr;
	}

	bool operator==(const InterIterator& other) const
	{
		return o_ptr == other.o_ptr;
	}

	bool operator!=(const InterIterator& other) const
	{
		return !(*this == other);
	}

private:
	PointerType o_ptr;
};


template <typename IOrder>
class Interpreter {
public:
	using ValueType = IOrder;
	using Iterator = InterIterator<Interpreter<IOrder>>;
	// using DevIterator = DeviceIterator<Interpreter<IOrder>>;

	Interpreter() {
		ReAlloc(2);
	}
	
	~Interpreter() {
		Clear();
	}
	
	void pushBack(IOrder&& newOrder) {
		if (o_size >= o_capacity)
			ReAlloc(o_capacity + o_capacity / 2);

		order_data[o_size] = std::move(newOrder);
		o_size++;
	}

	template<typename... Args>
	IOrder& addOrder(Args&&... args) {
		if (o_size >= o_capacity)
			ReAlloc(o_capacity + o_capacity / 2);
		new(&order_data[o_size]) IOrder(std::forward<Args>(args)...);
		return order_data[o_size++];
	}

	void Clear() {
		for (size_t i = 0; i < o_size; i++)
			order_data[i].~IOrder();

		o_size = 0;
	}

	IOrder& operator[](size_t index) {
		if (index >= o_size)
		{
			std::cout << "index out of range" << std::endl;
		}

		return order_data[index];
	}

	size_t Size() const { return o_size; }


	class DeviceIterator {
	public:
		using ValueType = typename Interpreter::ValueType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

		DeviceIterator(Iterator it, Iterator end, std::string o_device) : it(it), end(end), device(o_device) {
			while (true) {
				if (it == end) {
					break;
				}
				else if (it->getDevName() == device) {
					break;
				}
				it++;
			}
		}

		DeviceIterator& operator++() {
			while (true) {
				it++;
				if (it == end) {
					return *this;
				}
				else if (it->getDevName() == device) {
					return *this;
				}
			}
		}		

		ReferenceType operator[](int index) {
			return *(it + index);
		}

		PointerType operator->() {
			return it;
		}

		ReferenceType operator*() {
			return *it;
		}

		bool operator==(const DeviceIterator& other) const
		{
			return it == other.it;
		}

		bool operator!=(const DeviceIterator& other) const
		{
			return !(*this == other);
		}

	private:
		//PointerType o_ptr;
		Iterator it;
		Iterator end;
		std::string device;
	};

	class IntervalIterator {
	public:
		using ValueType = typename Interpreter::ValueType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

		bool inRange() {
			return (it->getTime() >= start && it->getTime() < stop);
		}

		IntervalIterator(Iterator it, Iterator end, t_point start, t_point stop) : it(it), end(end), start(start), stop(stop) {
			while (true) {
				if (it == end) {
					break;
				}
				else if (inRange()) {
					break;
				}
				it++;
			}
		}

		IntervalIterator& operator++() {
			while (true) {
				it++;
				if (it == end) {
					return *this;
				}
				else if (inRange()) {
					return *this;
				}
			}
		}

		ReferenceType operator[](int index) {
			return *(it + index);
		}

		PointerType operator->() {
			return it;
		}

		ReferenceType operator*() {
			return *it;
		}

		bool operator==(const IntervalIterator& other) const
		{
			return it == other.it;
		}

		bool operator!=(const IntervalIterator& other) const
		{
			return !(*this == other);
		}

	private:
		Iterator it;
		Iterator end;
		t_point start;
		t_point stop;
	};


	Iterator begin()
	{
		return Iterator(order_data);
	}

	Iterator end()
	{
		return Iterator(order_data + o_size);
	}
	
	DeviceIterator d_begin(std::string device)
	{
		return DeviceIterator(begin(), end(), device);
	}

	DeviceIterator d_end(std::string device)
	{
		return DeviceIterator(end(), end(), device);
	}

	IntervalIterator i_begin(t_point start, t_point stop)
	{
		return IntervalIterator(begin(), end(), start, stop);
	}

	IntervalIterator i_end(t_point start, t_point stop)
	{
		return IntervalIterator(end(), end(), start, stop);
	}

private:
	void ReAlloc(size_t newCapacity)
	{
		IOrder* newBlock = (IOrder*)::operator new(newCapacity * sizeof(IOrder));

		if (newCapacity < o_size)
			o_size = newCapacity;

		for (size_t i = 0; i < o_size; i++)
			new (&newBlock[i]) IOrder(std::move(order_data[i]));

		for (size_t i = 0; i < o_size; i++)
			order_data[i].~IOrder();

		::operator delete(order_data, o_capacity * sizeof(IOrder));
		order_data = newBlock;
		o_capacity = newCapacity;
	}

	IOrder* order_data = nullptr;
	size_t o_size = 0;
	size_t o_capacity = 0;
};


