#pragma once
#include "Greibach.h"
#include <stack>
#include "LexAnalysis.h"
#include <queue>
#include<iostream>
#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3
class my_stack_SHORT :public std::stack<short> {
public:
	using std::stack<short>::c;
};
static int FST_TRACE_n = -1;
static char rbuf[205], sbuf[205], lbuf[1024];



#define MFST_TRACE_START out<< std::setw(4)<<std::left<<"Шаг"<<":"\
								  << std::setw(20)<<std::left<<" Правило"\
								  << std::setw(30)<<std::left<<" Входная лента"\
								  << std::setw(20)<<std::left<<" Стек"\
								  << std::endl;

#define MFST_TRACE1		 out<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<rule.GetCRule(rbuf,nruleChain)\
								  << std::setw(30)<<std::left<<GetCLenta(lbuf,lentaPosition)\
								  << std::setw(20)<<std::left<<GetCSt(sbuf)\
								  << std::endl;

#define MFST_TRACE2		 out<< std::setw(4)<<std::left<<FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<GetCLenta(lbuf,lentaPosition)\
								  << std::setw(20)<<std::left<<GetCSt(sbuf)\
								  << std::endl;

#define MFST_TRACE3		 out<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<GetCLenta(lbuf,lentaPosition)\
								  << std::setw(20)<<std::left<<GetCSt(sbuf)\
								  << std::endl;

#define MFST_TRACE4(c)		out<<std::setw(4)<<std::left << ++FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;
#define MFST_TRACE5(c)		out<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;
#define MFST_TRACE6(c,k)	out<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left << c << k <<std::endl;

#define MFST_TRACE7			out<<std::setw(4)<<std::left << state.lentaPosition << ": "\
							<<std::setw(20)<< std::left << rule.GetCRule(rbuf,state.numRuleChain)\
							<<std::endl;




typedef my_stack_SHORT MFSTSTSTACK; // стек автомата 

namespace MFST
{
	struct MfstState // состояние автомата (для сохранения)
	{
		short nrule;
		short lentaPosition; // позиция на ленте
		short numRuleChain; // номер текущей цепочки, текущего правила
		MFSTSTSTACK stack;  // стек автомата
		MfstState();
		MfstState
		(
			short position, // позиция на ленте 
			MFSTSTSTACK pst, // стек автомата
			short pnRuleChain // номер текущей цепочки, текущего правила
		);
		MfstState(
			short position, // позиция на ленте 
			short pnrule,
			short pnRuleChain // номер текущей цепочки, текущего правила
		);
		MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);
	};
	struct Mfst // магазинный автомат
	{
		enum RC_STEP // код возврата функии step
		{
			NS_OK, // найдено правило и цепочка, цепочка записана в стек
			NS_NORULE, // не найдено правило грамматики (ошибка грамматики)
			NS_NORULECHAIN, // не найдена подходящая цепочка правила (ошибка в исходном коде)
			NS_ERROR, // неизвестный нетерминальный символ грамматики
			TS_OK, // тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK, // тек. символ ленты != вершине стека, восстановленно состояние
			LENTA_END,// текущая позиция ленты >= lentaSize
			SURPRISE// неожиданный код возврата(ошибка в step)
		};

		struct MfstDiagnosis // диагностика
		{
			short lentaPosition; // позиция в ленте
			RC_STEP rcStep; // код завершения шага
			short nrule; // номер правила
			short nruleChain; // номер цепочки правила
			MfstDiagnosis();
			MfstDiagnosis( // диагностика
				short plentaPosition, // позиция на ленте
				RC_STEP rcStep, // код завершения шага
				short pnrule, // номер правила
				short pnruleChain // номер цепочки правила
			);
		} diagnosis[MFST_DIAGN_NUMBER]; // последние самые глубокие сообщения 
		class my_stack_MfstState :public std::stack<MfstState> {
		public:
			using std::stack<MfstState>::c;
		};

		GRABALPHABET* lenta; // перекодированая (TS/NS) лента (из LEX)
		short lentaPosition; // текущая позиция на ленте
		short nrule; // номер текущего правила
		short nruleChain; // номер текущей цепочки, текущего правила
		short lentaSize; // размер ленты
		GRB::Greibach greibach; // грамматика Грейбах
		Lexis::LEX lex; // результат работы лексического анализатора
		MFSTSTSTACK st; // стек автомата
		//std::stack<MfstState> storeState; // стек для сохранения состояний
		my_stack_MfstState storeState;
		Mfst();
		Mfst(
			Lexis::LEX plex, // результат работы лексического анализатора
			GRB::Greibach pgrebach // грамматика Грейбах
		);
		char* GetCSt(char* buf); // получить содержимое стека
		char* GetCLenta(char* buf, short pos, short n = 25); // лента: n символов c pos
		char* GetDiagnosis(short n, char* buf); // получить n-ую строку диагностики или 0x00
		bool SaveState(std::ostream& out); // сохранить состояние автомата 
		bool ResetState(std::ostream& out); // восстановить состояние автомата
		bool PushChain(
			GRB::Rule::Chain chain // цепочка правила
		);
		RC_STEP step(std::ostream& out); // выполнить шаг автомата
		bool Start(std::ostream& out, std::ostream& outlog); // запустить автомат
		bool SaveDiagnosis(
			RC_STEP pprcStep // код завершения шага
		);
		void PrintRules(std::ostream& out); // вывести последовательность правил 
		struct Deducation // вывод
		{
			short size; // количество шагов в выводе
			short* nrules; // номера правил грамматики
			short* nruleChains; // номера цепочек правил грамматики
			Deducation() { size = 0; nrules = 0; nruleChains = 0; };
		} deducation;
		bool SaveDeducation(); // сохранить дерево вывода
	};
	void SyntacticAnalysis(Lexis::LEX lex, Log::LOG log, std::ostream& out);
}