#ifndef __LIST_LIB_HPP__
#define __LIST_LIB_HPP__

#include <iostream>
#include "data_interface.hpp"


constexpr auto DELIM_CHR = ' ';      



constexpr auto MAX_LIST_SIZE = 1000;  



template<typename T>
class List_node : public Data_interface<T>
{
public:
	class List_iterator 
	{
	public:
		List_iterator(Node * _node);
		bool operator==(const List_iterator & iter);
		bool operator!=(const List_iterator & iter);
		T operator*();
		void operator++();
	private:
		Node* current_node;
	};

	List_node(); 
	List_node(const unsigned int num_node);
	List_node(const unsigned int num_node, const unsigned int random_range = 1000);
	~List_node();

	void push(T new_value) override;
	void push_back(T new_value) override;

	T pop() override;
	T pop_back() override;

	void insert(T new_value, const unsigned int index);

	T get_node(const unsigned int index);
	size_t size() override;

	void print() override;
	void scan() override;

	friend std::ofstream &operator<<(std::ofstream& out, List_node<T>& list);

    List_iterator begin();
	List_iterator end();

	T* list_to_array(unsigned int& array_len);
private:
	struct Node 
	{
		Node* p_next_node;
		T value;
		Node() : p_next_node(nullptr)
        {}
        Node(T new_value) : value(new_value), p_next_node(nullptr)
        {}
	};
	Node* m_head_node;
};


template<typename T>
List_node<T>::List_iterator::List_iterator(Node* _node) : current_node(_node)
{}

template<typename T>
bool List_node<T>::List_iterator::operator==(const List_iterator& iter)
{
	if (this == &iter)
		return true;
	else
		return current_node == iter.current_node;
}

template<typename T>
bool List_node<T>::List_iterator::operator!=(const List_iterator& iter)
{
	return !(this == iter);
}

template<typename T>
T List_node<T>::List_iterator::operator*()
{
	if (current_node != nullptr)
		return current_node->value;
	else
		return T();
}

template<typename T>
void List_node<T>::List_iterator::operator++()
{
	if (current_node != nullptr)
		current_node = current_node->p_next_node;
}

template<typename T>
typename List_node<T>::List_iterator List_node<T>::begin()
{
	return List_iterator(m_head_node);
}

template<typename T>
typename List_node<T>::List_iterator List_node<T>::end()
{
	return List_iterator(nullptr);
}


template<typename T>
List_node<T>::List_node() : m_head_node(nullptr)
{}

template<typename T>
List_node<T>::List_node(const unsigned int num_node) : m_head_node(nullptr)
{
	for (size_t i = 0; i < num_node; ++i)
		push(0);
}


template<typename T>
List_node<T>::List_node(const unsigned int num_node, const unsigned int random_range) : m_head_node(nullptr)
{
	for (size_t i = 0; i < num_node; ++i)
		push(rand() % random_range);
}

template<typename T>
List_node<T>::~List_node()
{
	while (m_head_node != nullptr)
		pop();
}


template<typename T>
void List_node<T>::push(T new_value)
{
	if (Node* new_node = new Node(new_value))
	{
		new_node->p_next_node = m_head_node;
		m_head_node = new_node;
	}
}

template<typename T>
void List_node<T>::push_back(T new_value)
{
	if (Node* new_node = new Node(new_value))
	{
		new_node->p_next_node = nullptr;
		List_node* lastNode = m_head_node;
		while (lastNode->p_next_node!=nullptr)
			lastNode = lastNode->p_next_node;
		lastNode->p_next_node = new_node;
	}
}


template<typename T>
T List_node<T>::pop()
{
	if (m_head_node == nullptr)
		return T();
	List_node * lastNode = m_head_node;
	T lastValue = lastNode->value;
	m_head_node = m_head_node->p_next_node;
	free(lastNode);
	return lastValue;
}

template<typename T>
T List_node<T>::pop_back()
{
	if (m_head_node == nullptr)
		return T();
	if (m_head_node->p_next_node == nullptr)
	{
		free(m_head_node);
		m_head_node = nullptr;
		return T();
	}
	while (m_head_node->p_next_node->p_next_node != nullptr)
		m_head_node = m_head_node->p_next_node;
	List_node* lastNode = m_head_node;
	T lastValue = lastNode->p_next_node->value;
	free(lastNode->p_next_node);
	lastNode->p_next_node = nullptr;
	return lastValue;
}

template<typename T>
void List_node<T>::insert(T new_value, const unsigned int index)
{
	List_node* prev_node = m_head_node;
	size_t cnt = 0;
	while (cnt < index && prev_node != nullptr)
	{
		prev_node = prev_node->p_next_node;
		++cnt;
	}

	if (List_node* new_node = new List_node(new_value))
	{
		new_node->p_next_node = prev_node->p_next_node;
		prev_node->p_next_node = new_node;
	}
}

template<typename T>
T List_node<T>::get_node(const unsigned int index)
{
	List_node* index_node = m_head_node;
	size_t cnt = 0;
	while ((cnt++) < index && index_node != nullptr)
		index_node = index_node->p_next_node;
	return index_node->value;
}


template<typename T>
void List_node<T>::print()
{
	unsigned int list_size = size();
	for (size_t index = 0 ; index < list_size; ++index)
		std::cout << get_node(index) << DELIM_CHR;
	std::cout << std::endl; 
}

template<typename T>
void List_node<T>::scan()
{
	unsigned int size_list = 0; 
	std::cout << "Enter the number of nodes in the list: ";
	do
    {
		std::cin >> size_list;
	} while (size_list < 0 || MAX_LIST_SIZE < size_list);

	T value = 0;
	std::cout << "Enter the value [0] list node: ";
	std::cin >> value;
	push_back(value);
	for (size_t i = 1; i < size_list; ++i)
	{
		std::cout << "Enter the value [" << i << "] list node: ";
		std::cin >> value;
		push_back(value);
	}
}

template<typename T>
size_t List_node<T>::size()
{
	size_t size_list = 0;
	List_node* list_counter = m_head_node;
	while (list_counter != nullptr)
	{
		++size_list;
		list_counter = list_counter->p_next_node;
	}
	return size_list;
}

template<typename T>
T* List_node<T>::list_to_array(unsigned int& array_len)
{
	array_len = size();
	T* arr = new T[array_len];
	for (size_t i = 0; i < array_len; ++i)
		arr[i] = GetList(m_head_node, i)->value;
	return arr;
}

#endif
