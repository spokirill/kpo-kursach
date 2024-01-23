#pragma once
#include "pch.h"
namespace OUT
{
	struct OUT
	{
		std::ofstream* stream;

	};
	OUT getout(wchar_t outFile[]);
	void Close(OUT out);
}