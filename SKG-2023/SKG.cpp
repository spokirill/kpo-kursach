#include "pch.h"
#include <cwchar>
#include <locale>
#include "Parm.h"
#include "Log.h"
#include "Out.h"
#include "Mfst.h"
#include "Polish.h"
#include "Generation.h"
#include "SematicAnalisis.h"


int wmain(int argc, wchar_t* argv[])
{
	Log::LOG log = Log::INITLOG;
	OUT::OUT out;
	setlocale(LC_ALL, "ru");
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		out = OUT::getout(parm.logOut);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm);
		Log::WriteIn(log, in);

		Lexis::LEX lex = Lexis::LexicalAnaliz(in, log);
		LT::ShowTable(lex.lextable, *out.stream);
		IT::ShowTable(lex.idtable, *out.stream);
		Lexis::ShowTable(lex.lextable, *out.stream);
		MFST::SyntacticAnalysis(lex, log, *out.stream);

		Semantic::SemAnalyze(lex, log);
		Polish::StartPolish(lex);
		Lexis::Synchronization(lex);
		LT::ShowTable(lex.lextable, *out.stream);

		Gen::Generator Gener(lex.lextable, lex.idtable, parm.out);

		Log::Close(log);
		OUT::Close(out);
		LT::Delete(lex.lextable);
		IT::Delete(lex.idtable);

	}
	catch (Error::ERROR e)
	{
		
		Log::WriteError(log, e);
		return -1;
	}
	std::cout << "Все прошло успешно!\n";
	
	return 0;
}