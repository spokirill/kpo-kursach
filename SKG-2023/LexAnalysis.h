#pragma once
#include "pch.h"
#include "LT.h"
#include "IT.h"
#include "Log.h"
#include "In.h"
namespace Lexis
{
	struct LEX
	{
		IT::IdTable idtable;
		LT::LexTable lextable;
	};

	LEX LexicalAnaliz(In::IN in, Log::LOG log);
	void ShowTable(LT::LexTable& lextable, std::ostream& out);
	void Synchronization(Lexis::LEX& lex);
}
