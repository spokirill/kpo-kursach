#pragma once
#include "Error.h"
#include "Parm.h"
#define LEXEMA_FIXSIZE 1	//фиксированный размер лексем
#define LT_MAXSIZE 4096		//макс количество строк в таблице
#define LT_TI_NULLIDX 0xffffffff	//нет элемента ТИ
#define LEX_INTEGER		't'	//лексема для integer
#define LEX_BOOL        't' //лексема для bool
#define LEX_STRING		't'	//лексема для string
#define LEX_CHAR		't'	//терминальный символ типа данных 
#define LEX_LITERAL		'l'	//лексема для литерала
#define LEX_ID			'i'	//лексема для идентификатора
#define LEX_FUNCTION	'f'	//лексема для method
#define LEX_DECLARE		'd'	//лексема для var
#define LEX_RETURN		'r'	//return
#define LEX_WRITE		'o'	//write
#define LEX_MAINFUNC	'm'	//лексема для primary
#define LEX_CYCLE       'u' // лесема для cycle
#define LEX_IF          'w' // лесема для condition 
#define LEX_ELSE        '!' // лесема для otherwize
#define LEX_PROCEDURE	'p'	// лексема для procedure
#define LEX_BREAKL      'b' // hortab
#define LEX_SEMICOLON	';'	//;
#define LEX_COMMA		','	//,
#define LEX_LEFTBRACE	'{'	//{
#define LEX_BRACELET	'}'	//}
#define LEX_LEFTHESIS	'('	//(
#define LEX_RIGHTHESIS	')'	//)
#define LEX_OPERATOR    'v' // лексема для оператора
#define LEX_PLUS		'v'	//+
#define LEX_MINUS		'v'	//-
#define LEX_STAR		'v'	//*
#define LEX_VOPROS      'v' // ?
#define LEX_DIRSLASH	'v'	// /
#define LEX_RAV      	'='	//=
#define LEX_Zero            '0'//0

namespace LT	//ТАБЛИЦА ЛЕКСЕМ
{
	enum operations {
		NOT = -1,
		PLUSOPER,
		MINUSOPER,
		MULOPER,
		DIVOPER,
		DIVMODOPER,
		MOREOPER,
		LESSOPER,
		EQUOPER,
		NEQUOPER
	};
	struct Entry	//строка таблицы лексем
	{
		char lexema;	///лексема
		int strNumber;			///№ строки в исх. тексте
		int idxTI;	///индекс в ТИ или в LT_TI_NULLIDX
		int priority;
		operations op;
		Entry(char lexema, int strNumber, int idxTI)
		{
			this->lexema = lexema;
			this->strNumber = strNumber;
			this->idxTI = idxTI;
		}
		Entry(char lexema, int strNumber)
		{
			this->lexema = lexema;
			this->strNumber = strNumber;
		}
		Entry()
		{

		}
		~Entry()
		{

		}
	};

	struct LexTable		//экземпляр таблицы лексем
	{
		int maxSize;	///емкость ТЛ < LT_MAXSIZE
		int size;		///тек. размер ТЛ < maxsize
		Entry* table;	///массив строк ТЛ
	};

	LexTable Create( // создать таблицу лексем
		int size	/// емкость таблицы лексем < LT_MAXSIZE
	);
	void Add(				//добавить строку в таблицу лексем
		LexTable& lexTable, ///экземпл¤р таблицы лексем
		Entry entry			///строка таблицы лексем
	);

	Entry GetEntry(			//получить строку таблицы лексем
		LexTable& lexTable,	///экземпл¤р таблицы лексем
		int number				///номер получаемой строки
	);

	void ShowTable(LexTable& lextable, std::ostream& out);
	void Delete(LexTable& lexTable); //удалить таблицу лексем (освободить пам¤ть)
};
