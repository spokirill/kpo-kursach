#pragma once
#include "pch.h"
#include "Log.h"
#define maxWord 1000
#define sizeWord 20
namespace Div
{
	char** DivideText(char source[], int size);
	void cleanup(char source[], int size, Log::LOG logfile);
}