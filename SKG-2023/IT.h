#pragma once
#define ID_MAXSIZE 15
#define TI_MAXSIZE 4096 
#define TI_INT_DEFAULT 0x00000000
#define TI_STR_DEFAULT 0x00
#define TI_STR_MAXSIZE 255
#define TI_CHAR_MAXSIZE 1
#define TI_CHAR_DEFAILT 0x00
#define TI_NULLIDX 0xffffffff
#include "Parm.h"
namespace IT
{
	enum IDDATATYPE { NONE = 0, INT = 1, STR = 2, BOOL = 3, CHAR = 4, PROC = 5 };
	enum IDTYPE { V = 1024, F = 2048, P = 2049, L = 2050, OP = 5000 };
	struct Parms
	{
		IDDATATYPE idDataType; // тип параметра 
	};
	struct Entry // строка таблицы
	{
		
		int idxFirstLine; // индекс первой строки в таблице
		char id[ID_MAXSIZE]; // идентификатор
		IDDATATYPE idDataType; // тиб данных 
		IDTYPE idType; // тип
		int countOfPar = 0; // кол-во параметров(функция) 
		Parms parm[10]; // параметры 
		union
		{
			int vint; // значение инта
			char vchar[1];
			struct
			{
				int length;
				char str[TI_STR_MAXSIZE - 1];
			}vstr;// значение чара

		}value;

	};
	struct IdTable
	{
		int maxSize;
		int size;
		Entry* table;
	};
	IdTable Create(int size);
	void Add(IdTable& idTable, Entry entry);
	Entry GetEntry(IdTable& idTable, int number);
	int IsId(IdTable& idTable, char id[ID_MAXSIZE]);
	void Delete(IdTable& idTable);
	void ShowTable(IdTable& idTable, std::ostream& out);
	

}