#ifndef __STACK_LIB_HPP__
#define __STACK_LIB_HPP__

#include <iostream>
#include <cstring>
#include "data_interface.hpp"


template<typename, bool, size_t> class Stack_node;
template<typename T, size_t U> 
using Static_stack_node = Stack_node<T, true, U>;
template<typename T>
using Dyn_stack_node = Stack_node<T, false, 0>;


template<bool flag, typename T, typename U>
struct Select
{
	using Type = T;
};

template<typename T, typename U>
struct Select<false, T, U>
{
	using Type = U;
};

template<int i>
struct Int2type
{
	enum { value = i };
};



template<typename T, bool is_static, size_t MAX_STACK_SIZE = 1024>
class Stack_node : public Data_interface<T>
{
public:
	Stack_node();
	~Stack_node();
	void push(T newValue) override;
	T pop() override;
	T top() override;

	void clear() override;
	size_t size() override;

	void print() override;
	void scan() override;

	T* stack2array(unsigned int& array_len);
private:
	void destructor(Int2type<true>)
	{}
	
	void destructor(Int2type<false>) 
	{
		delete [] stack_data;
	}

	void push(T value, Int2type<true>)
	{
		if (index < MAX_STACK_SIZE - 1)
			stack_data[index++] = value;
	}
	
	void push(T value, Int2type<false>)
	{
		T* temp_data = new T[index + 2];
		for (size_t i = 0; i <= index; ++i)
			temp_data[i] = stack_data[i];
		delete [] stack_data;
		stack_data = temp_data;
		stack_data[index++] = value;
	}

	T pop(Int2type<true>)
	{
		if (index < 0)
			return T();
		T value = stack_data[index];
		stack_data[index--] = T();
		return value;
	}

	T pop(Int2type<false>)
	{
		if (index < 0)
			return T();
		T value = stack_data[index];
		T* temp_stack = new T[index];
		for (size_t i = 0; i < index; ++i)
			temp_stack[i] = stack_data[i];
		delete [] stack_data;
		--index;
		return value;
	}

	typename Select<is_static, T[MAX_STACK_SIZE], T*>::Type stack_data;
	size_t index;
};


template<typename T, bool is_static, size_t MAX_STACK_SIZE>
Stack_node<T, is_static, MAX_STACK_SIZE>::Stack_node() : index(0)
	{}

template<typename T, bool is_static, size_t MAX_STACK_SIZE>
Stack_node<T, is_static, MAX_STACK_SIZE>::~Stack_node()
{
	destructor(Int2type<is_static>());
}

template<typename T, bool is_static, size_t MAX_STACK_SIZE>
void Stack_node<T, is_static, MAX_STACK_SIZE>::push(T value)
{
	push(value, Int2type<is_static>());
}

template<typename T, bool is_static, size_t MAX_STACK_SIZE>
T Stack_node<T, is_static, MAX_STACK_SIZE>::pop()
{
	return pop(Int2type<is_static>());
}

template<typename T, bool is_static, size_t MAX_STACK_SIZE>
T Stack_node<T, is_static, MAX_STACK_SIZE>::top()
{
	return stack_data[index];
}

template<typename T, bool is_static, size_t MAX_STACK_SIZE>
void Stack_node<T, is_static, MAX_STACK_SIZE>::clear()
{
	while (index >= 0)
		pop();
}

template<typename T, bool is_static, size_t MAX_STACK_SIZE>
size_t Stack_node<T, is_static, MAX_STACK_SIZE>::size()
{
	return (index + 1);
}

template<typename T, bool is_static, size_t MAX_STACK_SIZE>
void Stack_node<T, is_static, MAX_STACK_SIZE>::print()
{
	for (size_t _index = 0; _index <= index; ++_index)
		std::cout << stack_data[_index] << ' ';
	std::cout << std::endl;
}

template<typename T, bool is_static, size_t MAX_STACK_SIZE>
void Stack_node<T, is_static, MAX_STACK_SIZE>::scan()
{
	std::cout << "Enter the number of stack elements: ";
	size_t stack_size;

	do
    {
		std::cin >> stack_size;
	} while (stack_size < 1);

	for (size_t _index = 0; _index < stack_size; ++_index)
	{
		std::cout << "Enter the value [" << _index << "] stack element: ";
		T input;
		std::cin >> input;
		push(input);
	}
}


#endif
