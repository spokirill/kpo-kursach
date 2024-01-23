#pragma once
#include "Error.h"
typedef short GRABALPHABET; // символы алфавита с грамматики терминалы >0, нетерминалы <0
namespace GRB
{
	struct Rule // правило в грамматике Грейбах
	{
		GRABALPHABET nn; // нетерминал(левый символ правила) < 0
		int idError; // идентификатор диагностического сообщения 
		short size; // кол-во цепочек - правых частей правила 

		struct Chain // цепочка (правая часть правила)
		{
			short size; // длина цепочки 
			GRABALPHABET* nt; // цепочка терминалов(>0) и нетерминалов (<0)
			Chain() { size = 0; nt = 0; };
			Chain(
				short psize, // кол-во символов в цепочке
				GRABALPHABET s, ... // символы (терминал или нетерминал)
			);
			char* GetCChain(char* b); // получить правую сторону правила 
			static GRABALPHABET Terminal(char t) { return GRABALPHABET(t); }; // терминал 
			static GRABALPHABET NTermenal(char n) { return -GRABALPHABET(n); }; // нетерминал
			static GRABALPHABET ISNS(char n) { return GRABALPHABET(n); };
			static bool IsTerm(GRABALPHABET s) { return s > 0; }; // терминал?
			static bool IsNotTerm(GRABALPHABET s) { return !IsTerm(s); }; // нетерминал?
			static char AlphabetToChar(GRABALPHABET s) { return IsTerm(s) ? char(s) : char(-s); }; // GRABALPHABET -> char
		}*chains; // массив цепочек - правых частей правила
		Rule() { nn = 0x00; size = 0; idError = 0; }
		Rule(
			GRABALPHABET pnn, // нетерминал (<0)
			int idError, // идентификатор диагностического сообщения (Error)
			short psize, // кол-во цепочек - правых частей правила 
			Chain c, ... // множество цепочек - правых частей правила 
		);

		char* GetCRule( // получить правило для распечатки 
			char* b, // буфер 
			short nchain // номер цепочки 
		);
		short GetNextChain( // получить следующую за j подходящую цепочку, вернуть номер или -1
			GRABALPHABET t, //  первый символ цепочки
			Rule::Chain& pchain, // возвращаемая цепочка 
			short j // номер цепочки 
		);
	};
	struct Greibach // грамматика Грейбах 
	{
		short size; // кол-во правил 
		GRABALPHABET startN; // стартовый символ 
		GRABALPHABET stBottomT; // дно стека 
		Rule* rules; // множество правил 
		Greibach() { size = 0; startN = 0; stBottomT = 0; rules = 0; };
		Greibach(
			GRABALPHABET pstartN, // стартовый символ 
			GRABALPHABET pstBottomT, // дно стека 
			short psize, // кол-во правил 
			Rule r, ... // правила
		);
		short GetRule( // вернет номер парвила или -1 
			GRABALPHABET pnn, // левый символ правила 
			Rule& prule // правило 
		);
		Rule GetRule(short n); // по номеру - праило 
	};
	Greibach GetGreibach(); // получить грамматику 

}
