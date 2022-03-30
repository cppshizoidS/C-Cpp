#pragma once
#include <string>
class Exception
{
private:
	int code;
	std::string message;
public:
	Exception(int code, std::string message);
	~Exception();
	void Show();
};

