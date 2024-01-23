#include <iostream>
#include <Windows.h>

extern "C"
{
	void BREAKL()
	{
		std::cout << std::endl;
	}

	void OutputChar(char a)
	{
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		std::cout << a;
	}

	void OutputInt(int a)
	{
		std::cout << a;
	}

	
	void OutputBool(int a)
	{
		if (a == 1)
			std::cout << "true";
		else
			std::cout << "false";
	}

	

	

	int randGen()
	{
		return rand();
	}
}