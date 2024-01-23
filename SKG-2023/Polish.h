#pragma once
#include "pch.h"
#include "LexAnalysis.h"
#include "Error.h"

namespace Polish
{
	bool PolishNotation(int i, Lexis::LEX& lex);
	void StartPolish(Lexis::LEX& lex);

}
