#include "pch.h"
#include "IT.h"
#include "Out.h"
#include "Error.h"

namespace IT
{
	IdTable Create(int size)
	{
		IdTable* newIdTable = new IdTable;
		if (size > TI_MAXSIZE)
			throw ERROR_THROW(203);
		newIdTable->size = 0;
		newIdTable->maxSize = size;
		newIdTable->table = new Entry[size];
		return *newIdTable;
	}
	void Add(IdTable& idTable, Entry entry)
	{
		if (idTable.size + 1 > idTable.maxSize)
			throw ERROR_THROW(204);
		idTable.table[idTable.size] = entry;
		idTable.size++;
	}
	Entry GetEntry(IdTable& idTable, int number)
	{
		return idTable.table[number];
	}
	int IsId(IdTable& idTable, char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idTable.size; i++) // mb -1
		{
			if (strcmp(idTable.table[i].id, id) == 0) return i;
		}
		return TI_NULLIDX;
	}
	void Delete(IdTable& idTable)
	{
		delete[] idTable.table;
	}
	void ShowTable(IdTable& idtable, std::ostream& out)
	{
		out << std::endl;
		out << "ID Table" << std::endl;
		int i, j, numberOP = 0;
		out << std::setfill('-') << std::setw(87) << '-' << std::endl;
		out << " №" << "   | " << "Identifier" << "    | " << "Data Type" << "  | " << "Identifier Type" << "    | " << "Index in LT" << " | " << "Value " << std::endl;
		out << std::setw(87) << '-' << std::endl;
		for (i = 0, j = 0; i < idtable.size; i++, j++)
		{
			if (idtable.table[i].idType == OP)
			{
				numberOP++;
				j--;
				continue;
			}
			out << std::setfill('0') << std::setw(4) << std::right << j << " | ";
			out << std::setfill(' ') << std::setw(13) << std::left << idtable.table[i].id << " | ";

			switch (idtable.table[i].idDataType)
			{
			case INT: out << std::setw(10) << std::left;
				if (idtable.table[i].idType == OP) out << "-" << " | ";
				else out << "integer" << " | "; break;
			case STR: out << std::setw(10) << std::left;
				if (idtable.table[i].idType == OP) out << "-" << " | ";
				else out << "string" << " | "; break;
			case CHAR: out << std::setw(10) << std::left;
				if (idtable.table[i].idType == OP) out << "-" << " | ";
				else out << "symbol" << " | "; break;
			case BOOL: out << std::setw(10) << std::left;
				if (idtable.table[i].idType == OP) out << "-" << " | ";
				else out << "boolean" << " | "; break;
			case PROC: out << std::setw(10) << std::left;
				if (idtable.table[i].idType == OP) out << "-" << " | ";
				else out << "void" << " | "; break;
			default: out << std::setw(10) << std::left << "unknown" << " | "; break;
			}
			switch (idtable.table[i].idType)
			{
			case V: out << std::setw(18) << std::left << "переменная" << " | "; break;
			case F:
				if (idtable.table[i].idDataType == PROC)
					out << std::setw(18) << std::left << "процедура" << " | ";
				else
					out << std::setw(18) << std::left << "метод" << " | ";
				break;
			case P: out << std::setw(18) << std::left << "параметр" << " | "; break;
			case L: out << std::setw(18) << std::left << "литерал" << " | "; break;
			case OP: out << std::setw(18) << std::left << "оператор" << " | ";
				numberOP++;
				break;
			default: out << std::setw(18) << std::left << "unknown" << " | "; break;
			}
			out << std::setw(11) << std::left << idtable.table[i].idxFirstLine << " | ";
			if (idtable.table[i].idDataType == INT && (idtable.table[i].idType == V || idtable.table[i].idType == L))
				out << std::setw(18) << std::left << idtable.table[i].value.vint;
			else if (idtable.table[i].idDataType == STR && (idtable.table[i].idType == V || idtable.table[i].idType == L))
				out << "[" << idtable.table[i].value.vstr.length << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else if (idtable.table[i].idDataType == CHAR && (idtable.table[i].idType == V || idtable.table[i].idType == L))
				out << "\'" << idtable.table[i].value.vchar << "\'";
			else if (idtable.table[i].idDataType == BOOL && (idtable.table[i].idType == V || idtable.table[i].idType == L))
				out << std::setw(18) << std::left << idtable.table[i].value.vint;
			else
				out << "-";
			out << std::endl;
		}
		out << std::setfill('-') << std::setw(87) << '-' << std::endl;
		out << "Количество идентификаторов: " << i - numberOP << std::endl;
		out << std::setw(87) << '-' << std::endl;
	}
	
}