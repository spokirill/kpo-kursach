#include "pch.h"
#include "LT.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
using namespace Log;
using namespace std;
namespace LT
{
	LexTable Create(int size)
	{
		LexTable* newLexTable = new LexTable;
		if (size > LT_MAXSIZE)
			throw ERROR_THROW(201);
		newLexTable->maxSize = size;
		newLexTable->size = 0;
		newLexTable->table = new Entry[size];
		return *newLexTable;
	}

	void Add(LexTable& lexTable, Entry entry)
	{
		if (lexTable.size + 1 > lexTable.maxSize)
			throw ERROR_THROW(202);
		lexTable.table[lexTable.size] = entry;
		lexTable.size++;
	}

	Entry GetEntry(LexTable& lexTable, int number)
	{
		return lexTable.table[number];
	}

	void Delete(LexTable& lexTable)
	{
		delete[] lexTable.table;
	}

	void ShowTable(LexTable& lextable, std::ostream& out) {
		out << "\nÏÐÎÌÅÆÓÒÎ×ÍÎÅ ÏÐÅÄÑÒÀÂËÅÍÈÅ ÊÎÄÀ:\n";
		int line = 2;
		out << "0001| ";
		for (int i = 0; i < lextable.size; i++) {
			if (lextable.table[i].lexema == ';') {
				out << lextable.table[i].lexema << endl;
				if (i + 1 == lextable.size) break;
				line > 999 ? out << line << "| " : line > 99 ? out << "0" << line << "| " : line > 9 ? out << "00" << line <<
					"| " : out << "000" << line << "| ";
				line++;
			}
			else out << lextable.table[i].lexema;
		}
	}
}
