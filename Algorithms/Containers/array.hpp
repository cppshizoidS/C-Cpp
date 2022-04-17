#ifndef __ARRAY_LIB_HPP__
#define __ARRAY_LIB_HPP__

#include <iostream>
#include <ctime>



constexpr auto DELIM_CHR = ' ';         


constexpr auto MAX_ARR_SIZE = 1000;       
constexpr auto RANDOM_RANGE = 100;

template<typename T>
T* init_arr(const unsigned int _len)
{
	if((_len < 1) || (MAX_ARR_SIZE < _len))
		return nullptr;
	
	T* arr = new T [_len];
	
	#ifdef RANDOM_RANGE
		for (size_t i = 0; i < _len; ++i)
			arr[i] = rand() % RANDOM_RANGE;
	#else
		memset(arr, 0, _len);
	#endif
	
	return arr;
}

template<typename T>
T** init_arr2(const unsigned int _row, const unsigned int _column)
{
	if(((_row * _column) < 1) || (MAX_ARR_SIZE < (_row * _column)))
		return nullptr;

	T** arr = new T* [_row];
	arr[0] = new T [_row * _column];
	
	for (size_t i = 0; i < _row; ++i)
	{
		arr[i] = arr[0] + _column*i;
		
	#ifdef RANDOM_RANGE
		for (size_t j = 0; j < _column; ++j)
			arr[i][j] = rand() % RANDOM_RANGE;
	#else
		memset(arr[i], 0, _column);
	#endif
	}
	
	return arr;
}


template<typename T>
void print_arr(T* arr, const unsigned int _len)
{
	for (size_t i = 0; i < _len; ++i)
		std::cout << arr[i] << DELIM_CHR;   
	std::cout << std::endl;                 
}

template<typename T>
void print_arr2(T** arr, const unsigned int _row, const unsigned int _column)
{
	for (size_t i = 0; i < _row; ++i)
	{
		for (size_t j = 0; j < _column; ++j)
			std::cout << arr[i][j] << DELIM_CHR; 
		std::cout << std::endl;                        
	}
}


template<typename T>
T* scan_arr(unsigned int & p_len)
{
	std::cout << "Enter the length of the array: ";
	do
    {
		std::cin >> p_len;
	} while (p_len < 0 || MAX_ARR_SIZE < p_len);
	T* arr = init_arr<T>(p_len);
	for (size_t i = 0; i < p_len; ++i)
	{
		std::cout << "Enter the value [" << i << "] element: ";
		std::cin >> arr[i];
	}
	return arr;
}


template<typename T>
T** scan_arr2(unsigned int& p_row, unsigned int& p_column)
{
	std::cout << "Enter the number of rows in the array: ";
	do
    {
		std::cin >> p_row;
	} while (p_row < 0 || MAX_ARR_SIZE < p_row);
	
	std::cout << "Enter the number of rows in the array: ";
	do
    {
		std::cin >> p_column;
	} while (p_column < 0 || MAX_ARR_SIZE < p_column);
	
	T** arr = init_arr2<T>(p_row, p_column);
	
	for (size_t i = 0; i < p_row; ++i)
	{
		for (size_t j = 0; j < p_column; ++j)
		{
			std::cout << "Enter the value [" << i << "][" << j << "] element: "; 
			std::cin >> arr[i][j];
		}
		std::cout << std::endl; 
	}
	return arr;
}



template<typename T>
void quick_sort(T* arr, const unsigned int _left, const unsigned int _right, int (*comp)(const T, const T))
{
	if(_left >= _right)
		return;
	swap(arr[_left], arr[(_left + _right)/2]);
	unsigned int last_left = _left;
	for (size_t index = _left + 1; index <= _right; ++index)
		if(comp(arr[index], arr[_left]) > 0)
			swap(arr[index], arr[++last_left]);

	swap(arr[_left], arr[last_left]);

	quick_sort(arr, _left, last_left - 1, comp);
	quick_sort(arr, last_left + 1, _right, comp);
}


template<typename T>
void shell_sort(T* arr, const unsigned int len, int (*comp)(const T, const T))
{
	for (size_t dist = len/2; dist > 0; dist /= 2)
		for (size_t i = 0; i < len - dist; ++i)
			for (long j = i; j >= 0 && comp(arr[j], arr[j + dist]) > 0; j -= dist)
				swap(arr[j], arr[j + dist]);
}

template<typename T>
void merge_sort(T* arr, const size_t len, int (*comp)(const T, const T))
{
	if(len < 2)
		return;
	size_t middle = len/2;
	merge_sort(arr, middle, comp);
	merge_sort(&arr[middle], len/2, comp);
	T* buf = new T[len];
	for (size_t left = 0, right = middle, i = 0; i < len; ++i)
		if((right >= len) || (left < middle && comp(arr[left], arr[right])))
			buf[i] = arr[left++];
		else
			buf[i] = arr[right++];
	for (size_t i = 0; i < len; ++i)
		arr[i] = buf[i];
	delete [] buf;
}


template<typename T>
void swap(T & a, T & b)
{
	T old_element = std::move(a);
	a = std::move(b);
	b = std::move(old_element);
}

size_t _strlen(char* s)
{
	size_t len = 0;
	while (s[len++] != 0);
	return len;
}

void _strcpy(char* out, char* in)
{
	while ((*(out++) = *(in++)) != 0);
}

char* _strtok(char* src, char* delim)
{
	auto isdelim = [&delim](char c) -> bool {
		for (size_t i = 0; i <= _strlen(delim); ++i)
			if (c == delim[i])
				return true;
		return false;
	};
	
	static char* buf = nullptr;
	static size_t index, len;
	if (src != nullptr)
	{
		if (buf != nullptr)
		{
			delete buf;
			buf = nullptr;
		}
		len = _strlen(src);
		buf = new char[len];
		_strcpy(buf, src);
		index = 0;
	}
	
	size_t left = EOF;
	for (; index <= len; ++index)
	{
		if (isdelim(buf[index]))
		{
			if (left >= 0 && (index-1) >= left)
			{
				buf[index++] = 0;
				return &buf[left];
			}
		} else if (static_cast<int>(left) == EOF)
			left = index;
	}
	
	if (buf != nullptr)
	{
		delete buf;
		buf = nullptr;
	}
	
	return nullptr;
}


template<typename T>
int resize_arr2(T** (&arr), const size_t old_n, const size_t old_m, const size_t new_n, const size_t new_m)
{
	if (new_m * new_n != old_m * old_n)
		return 1;
	
	T* buf = new T[old_n * old_m]; //malloc(old_n * old_m * sizeof(T));
	for (size_t i = 0; i < old_n; ++i)
		for (size_t j = 0; j < old_m; ++j)
			buf[old_m*i + j] = arr[i][j];
	delete arr[0];
	delete arr;
	
	arr = new T*[new_n];
	arr[0] = new T[new_n * new_m];
	for (size_t i = 0; i < new_n; ++i)
	{
		arr[i] = arr[0] + new_m * i;
		for (size_t j = 0; j < new_m; ++j)
			arr[i][j] = buf[new_m*i + j];
	}
	
	delete buf;
	return 0;
}

#endif
