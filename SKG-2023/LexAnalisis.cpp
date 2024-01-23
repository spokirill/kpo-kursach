#include "pch.h"
#include "LexAnalysis.h"
#include "Graphs.h"
#include "Div.h"
#define DIV			'|'
#define SPACE		' '
#define PLUS		'+'
#define MINUS		'-'
#define STAR		'*'
#define DIRSLASH	'/'
#define MOD			'%'
#define MORE		'>'
#define LESS		'<'
#define EQU			'~'
#define NEQU		'!'
#define EQUAL		'='
#define EMPTYSTR ""
using namespace std;

namespace Lexis
{

	LEX LexicalAnaliz(In::IN in, Log::LOG log)
	{

		LEX lex;
		LT::LexTable lexTable = LT::Create(LT_MAXSIZE);
		IT::IdTable idTable = IT::Create(TI_MAXSIZE);
		Div::cleanup(in.text, in.size, log);

		char** word = new char* [maxWord];
		for (int i = 0; i < maxWord; i++)
			word[i] = new char[sizeWord] {NULL};


		while (true)
		{
			word = Div::DivideText(in.text, in.size);
			if (word != NULL) break;
		}

		/*	for (int i = 0; word[i]; i++) {
				cout << word[i];
			}
			cout << endl;*/
		int NOWIDXFUNC = 0;

		int indexLex = 0;
		int literalCounter = 1;
		int line = 1;
		int position = 0;
		bool findMain = false;
		int mainCounter = 0;
		bool findDeclaration = false;
		bool findType = false;
		bool findProc = false;
		int is_cycle = 0;
		int parmsCounter = 0;

		std::stack<std::string> functions;
		char* bufprefix = new char[10] { "" };
		char* L = new char[2] { "L" };
		char* bufL = new char[TI_STR_MAXSIZE];
		char* nameLiteral = new char[10] { "" };
		char* charclit = new char[10] { "" };

		bool findFunc = false;
		bool findParm = false;
		IT::Entry entryIT;

		for (int i = 0; word[i] != NULL; i++, indexLex++)
		{
			bool findSameId = false;
			position += strlen(word[i]);
			switch (word[i][0]) // для ускорения смотрим по первому символу 
			{
			case 'v':
			{
				if (FST::execute(FST::FST(word[i], FST_DECLARE)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_DECLARE, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					findDeclaration = true;
					continue;
				}
			} break;
			case 'i':
			{

				if (FST::execute(FST::FST(word[i], FST_INTEGER)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_INTEGER, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					entryIT.idDataType = IT::INT;
					findType = true;
					continue;
				}
			}break;
			case 'b':
			{
				if (FST::execute(FST::FST(word[i], FST_BOOL)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_BOOL, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					entryIT.idDataType = IT::BOOL;
					findType = true;
					continue;
				}

			}break;
			case 's':
			{
				if (FST::execute(FST::FST(word[i], FST_STRING)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_STRING, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					entryIT.idDataType = IT::STR;
					strcpy(entryIT.value.vstr.str, EMPTYSTR);
					findType = true;
					continue;
				}
				else if (FST::execute(FST::FST(word[i], FST_CHAR)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_CHAR, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					entryIT.idDataType = IT::CHAR;
					findType = true;
					continue;
				}
				else if (FST::execute(FST::FST(word[i], FST_STRLEN)) || FST::execute(FST::FST(word[i], FST_ATOI)))
				{
					if (IT::IsId(idTable, word[i]) == TI_NULLIDX) {
						entryIT.idType = IT::F;
						entryIT.idDataType = IT::INT;
						entryIT.parm->idDataType = IT::STR;
						entryIT.countOfPar = 1;
						entryIT.idxFirstLine = indexLex;
						strcpy(entryIT.id, word[i]);
						IT::Add(idTable, entryIT);
						entryIT = {};
					}
					LT::Entry* entryLT = new LT::Entry(LEX_ID, line, IT::IsId(idTable, word[i]));
					LT::Add(lexTable, *entryLT);
					continue;
				}
				else if (FST::execute(FST::FST(word[i], FST_STRCMP)))
				{
					if (IT::IsId(idTable, word[i]) == TI_NULLIDX) {
						entryIT.idType = IT::F;
						entryIT.idDataType = IT::INT;
						entryIT.parm[0].idDataType = IT::STR;
						entryIT.parm[1].idDataType = IT::STR;
						entryIT.countOfPar = 2;
						entryIT.idxFirstLine = indexLex;
						strcpy(entryIT.id, word[i]);
						IT::Add(idTable, entryIT);
						entryIT = {};
					}
					LT::Entry* entryLT = new LT::Entry(LEX_ID, line, IT::IsId(idTable, word[i]));
					LT::Add(lexTable, *entryLT);
					continue;
				}
			}break;
			case 'f':
			{

				if (FST::execute(FST::FST(word[i], FST_FALSE)))
				{
					for (int k = 0; k < idTable.size; k++) {
						if (idTable.table[k].value.vint == 0 && idTable.table[k].idType == IT::L && idTable.table[k].idDataType == IT::BOOL) {
							LT::Entry* entryLT = new LT::Entry(LEX_LITERAL, line, k);
							LT::Add(lexTable, *entryLT);
							findSameId = true;
							break;
						}
					}

					if (findSameId) continue;

					entryIT.idType = IT::L;
					entryIT.idDataType = IT::BOOL;
					entryIT.value.vint = 0;
					entryIT.idxFirstLine = indexLex;
					_itoa_s(literalCounter++, charclit, sizeof(char) * 10, 10);
					strcpy(bufL, L);
					word[i] = strcat(bufL, charclit);
					strcpy(entryIT.id, word[i]);
					IT::Add(idTable, entryIT);
					entryIT = {};

					LT::Entry* entryLT = new LT::Entry(LEX_LITERAL, line, IT::IsId(idTable, word[i]));
					LT::Add(lexTable, *entryLT);
					continue;

				}

			}break;
			case 'm': // возможно будет massiv 
			{
				if (FST::execute(FST::FST(word[i], FST_FUNCTION)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_FUNCTION, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					findFunc = true;
					continue;
				}
			}break;
			case 't':
			{
				if (FST::execute(FST::FST(word[i], FST_TRUE)))
				{
					for (int k = 0; k < idTable.size; k++) {
						if (idTable.table[k].value.vint == 1 && idTable.table[k].idType == IT::L && idTable.table[k].idDataType == IT::BOOL) {
							LT::Entry* entryLT = new LT::Entry(LEX_LITERAL, line, k);
							LT::Add(lexTable, *entryLT);
							findSameId = true;
							break;
						}
					}

					if (findSameId) continue;

					entryIT.idType = IT::L;
					entryIT.idDataType = IT::BOOL;
					entryIT.value.vint = 1;
					entryIT.idxFirstLine = indexLex;
					_itoa_s(literalCounter++, charclit, sizeof(char) * 10, 10);
					strcpy(bufL, L);
					word[i] = strcat(bufL, charclit);
					strcpy(entryIT.id, word[i]);
					IT::Add(idTable, entryIT);
					entryIT = {};

					LT::Entry* entryLT = new LT::Entry(LEX_LITERAL, line, IT::IsId(idTable, word[i]));
					LT::Add(lexTable, *entryLT);
					continue;
				}

			}break;
			case 'r':
			{
				if (FST::execute(FST::FST(word[i], FST_RETURN)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_RETURN, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					continue;
				}
			}break;
			case 'w':
			{
				if (FST::execute(FST::FST(word[i], FST_WRITE)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_WRITE, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					continue;
				}
			}break;
			case 'p':
			{
				if (FST::execute(FST::FST(word[i], FST_MAIN)))
				{
					strcpy(entryIT.id, word[i]);
					entryIT.idType = IT::F;
					entryIT.idxFirstLine = indexLex;
					entryIT.idDataType = IT::INT;
					IT::Add(idTable, entryIT);
					entryIT = {};

					LT::Entry* entryLT = new LT::Entry(LEX_MAINFUNC, line, IT::IsId(idTable, word[i]));
					LT::Add(lexTable, *entryLT);
					functions.push(word[i]);
					findMain = true;
					mainCounter++;
					continue;
				}
				else if (FST::execute(FST::FST(word[i], FST_PROCEDURE)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_PROCEDURE, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					findProc = true;
					findFunc = true;
					continue;
				}

			}break;
			case 'c':
			{
				if (FST::execute(FST::FST(word[i], FST_IF)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_IF, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					is_cycle++;
					continue;
				}
				else if (FST::execute(FST::FST(word[i], FST_CYCLE)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_CYCLE, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					is_cycle++;
					continue;
				}

			}break;
			case 'o':
			{
				if (FST::execute(FST::FST(word[i], FST_ELSE)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_ELSE, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					is_cycle++;
					continue;
				}

			}break;
			case 'h':
			{
				if (FST::execute(FST::FST(word[i], FST_BREAKL)))
				{
					LT::Entry* entryLT = new LT::Entry(LEX_BREAKL, line, LT_TI_NULLIDX);
					LT::Add(lexTable, *entryLT);
					continue;
				}

			}break;
			case '\'':
				if (FST::execute(FST::FST(word[i], FST_CHARLIT)))
				{
					int length = strlen(word[i]);
					if (length > 3)// если символьный литерал больше 1
					{
						std::cout << Error::geterror(303).message << ", строка: " << line << "\n";
						throw ERROR_THROW_IN(303, line, -1);
					}
					for (int k = 0; k < length; k++)
						word[i][k] = word[i][k + 1];
					word[i][length - 2] = 0;

					for (int k = 0; k < idTable.size; k++) {
						if (!(strcmp(idTable.table[k].value.vchar, word[i]))) {
							findSameId = true;
							LT::Entry* entryLT = new LT::Entry(LEX_LITERAL, line, k);
							LT::Add(lexTable, *entryLT);
							break;
						}
					}

					if (findSameId) continue;

					strcpy(entryIT.value.vchar, word[i]);
					entryIT.idType = IT::L;
					entryIT.idDataType = IT::CHAR;
					entryIT.idxFirstLine = indexLex;


					_itoa_s(literalCounter++, charclit, sizeof(char) * 10, 10);
					strcpy(bufL, L);
					nameLiteral = strcat(bufL, (char*)charclit);
					strcpy(entryIT.id, nameLiteral);
					IT::Add(idTable, entryIT);
					entryIT = {};

					int k = 0;
					for (k = 0; k < idTable.size; k++) {
						if (!(strcmp(idTable.table[k].value.vchar, word[i])))
							break;
					}
					LT::Entry* entryLT = new LT::Entry(LEX_LITERAL, line, k);
					LT::Add(lexTable, *entryLT);

					continue;
				}
				std::cout << Error::geterror(308).message << ", строка: " << line << "\n";
				throw ERROR_THROW_IN(308, line, -1);
				break;
			case '\"':
			{
				if (FST::execute(FST::FST(word[i], FST_STRLIT)))
				{
					int length = strlen(word[i]);
					if (length > 255)
						throw ERROR_THROW_IN(318, line, -1);
					for (int k = 0; k < length; k++)
						word[i][k] = word[i][k + 1];
					word[i][length - 2] = 0;

					for (int k = 0; k < idTable.size; k++) {
						if (!(strcmp(idTable.table[k].value.vstr.str, word[i]))) {
							findSameId = true;
							LT::Entry* entryLT = new LT::Entry(LEX_LITERAL, line, k);
							LT::Add(lexTable, *entryLT);
							break;
						}
					}
					if (findSameId) continue;

					strcpy(entryIT.value.vstr.str, word[i]);
					entryIT.value.vstr.length = length - 2;
					entryIT.idType = IT::L;
					entryIT.idDataType = IT::STR;
					entryIT.idxFirstLine = indexLex;

					_itoa_s(literalCounter++, charclit, sizeof(char) * 10, 10);
					strcpy(bufL, L);
					nameLiteral = strcat(bufL, (char*)charclit);
					strcpy(entryIT.id, nameLiteral);
					IT::Add(idTable, entryIT);
					entryIT = {};

					int k = 0;
					for (k = 0; k < idTable.size; k++) {
						if (!(strcmp(idTable.table[k].value.vstr.str, word[i])))
							break;
					}
					LT::Entry* entryLT = new LT::Entry(LEX_LITERAL, line, k);
					LT::Add(lexTable, *entryLT);
					continue;
				}
			}break;
			default: break;
			}

			if (FST::execute(FST::FST(word[i], FST_ID)))
			{
				if (findParm)
				{
					entryIT.idType = IT::P;
					parmsCounter++;
				}
				else if (findFunc)
				{
					NOWIDXFUNC = idTable.size;
					if (findProc)
						entryIT.idDataType = IT::PROC;
					entryIT.idType = IT::F;
					functions.push(word[i]);
				}
				else
				{
					entryIT.idType = IT::V;
					int idx = IT::IsId(idTable, word[i]);
					if (idx != TI_NULLIDX)
					{
						LT::Entry* entryLT = new LT::Entry(LEX_ID, line, idx);
						LT::Add(lexTable, *entryLT);
						entryIT = { };
						continue;
					}
					if (entryIT.idDataType == IT::INT)
						entryIT.value.vint = TI_INT_DEFAULT;
					else if (entryIT.idDataType == IT::STR)
					{
						entryIT.value.vstr.length = 0;
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
					}
					else if (entryIT.idDataType == IT::CHAR)
					{
						memset(entryIT.value.vchar, TI_STR_DEFAULT, sizeof(char));
					}
				}

				entryIT.idxFirstLine = indexLex;
				if (entryIT.idType != IT::F && !functions.empty()) {
					strcpy(bufprefix, functions.top().c_str());
					word[i] = strcat(bufprefix, word[i]);
				}
				strcpy(entryIT.id, word[i]);
				int idx = IT::IsId(idTable, word[i]);
				if (idx == TI_NULLIDX)
					IT::Add(idTable, entryIT);
				LT::Entry* entryLT = new LT::Entry(LEX_ID, line, IT::IsId(idTable, word[i]));
				LT::Add(lexTable, *entryLT);
				entryIT = { };
				continue;
			}
			else if (FST::execute(FST::FST(word[i], FST_INTLIT)))
			{
				long value = 0;
				int sign = 1;
				int h = 0;
				int l = 0;
				bool isbin = false;
				if (word[i][0] == '^' || word[i][0] == 'i')
				{
					h = strlen(word[i]);
					l = 1;
				}
				else
					h = strlen(word[i]);


				char* buff = new char[h];
				for (int j = 0; j <= h; j++)
				{
					buff[j] = word[i][l];
					l++;
				}

				if (word[i][0] == '^')
				{
					isbin = true;
					int k = 1;
					int end = 0;
					if ((h - 1) % 8 == 0 && buff[0] == '1')
					{
						sign = -1;
						end = 1;
					}
					for (int g = h - 1; g >= end; g--)
					{
						if (buff[g] == '1' || buff[g] == '0')
						{
							value += k * (buff[g] - '0');
							k *= 2;
						}
						else if (buff[g] != '\0')
						{
							std::cout << Error::geterror(207).message << ", строка: " << line << "\n";
							throw ERROR_THROW_IN(207, line, -1);
						}

					}
					value *= sign;
				}

				if (word[i][0] == 'i')
					sign = -1;
				if (!isbin)
					value = sign * strtol(buff, nullptr, 0);

				for (int k = 0; k < idTable.size; k++) {
					if (idTable.table[k].value.vint == value && idTable.table[k].idType == IT::L && idTable.table[k].idDataType == IT::INT) {
						LT::Entry* entryLT = new LT::Entry(LEX_LITERAL, line, k);
						LT::Add(lexTable, *entryLT);
						findSameId = true;
						break;
					}
				}

				if (findSameId) continue;
				entryIT.idType = IT::L;
				entryIT.idDataType = IT::INT;
				entryIT.value.vint = value;
				entryIT.idxFirstLine = indexLex;
				_itoa_s(literalCounter++, charclit, sizeof(char) * 10, 10);
				strcpy(bufL, L);
				word[i] = strcat(bufL, charclit);
				strcpy(entryIT.id, word[i]);
				IT::Add(idTable, entryIT);
				entryIT = {};

				LT::Entry* entryLT = new LT::Entry(LEX_LITERAL, line, IT::IsId(idTable, word[i]));
				LT::Add(lexTable, *entryLT);
				continue;
			}

			else if (FST::execute(FST::FST(word[i], FST_OPERATOR)))
			{
				strcpy(entryIT.id, word[i]);
				entryIT.idxFirstLine = indexLex;
				entryIT.idType = IT::OP;
				if (IT::IsId(idTable, word[i]) == TI_NULLIDX)
					IT::Add(idTable, entryIT);
				entryIT = { };
				LT::Entry* entryLT = new LT::Entry(LEX_OPERATOR, line, IT::IsId(idTable, word[i]));

				switch (word[i][0])
				{
				case PLUS:
					(*entryLT).priority = 2;
					entryLT->op = LT::operations::PLUSOPER;
					break;
				case MINUS:
					(*entryLT).priority = 2;
					entryLT->op = LT::operations::MINUSOPER;
					break;
				case DIRSLASH:
					(*entryLT).priority = 3;
					entryLT->op = LT::operations::DIVOPER;
					break;
				case STAR:
					(*entryLT).priority = 3;
					entryLT->op = LT::operations::MULOPER;
					break;
				case MOD:
					(*entryLT).priority = 3;
					entryLT->op = LT::operations::DIVMODOPER;
					break;
				case MORE:
					(*entryLT).priority = 0;
					entryLT->op = LT::operations::MOREOPER;
					break;
				case LESS:
					(*entryLT).priority = 0;
					entryLT->op = LT::operations::LESSOPER;
					break;
				case EQU:
					(*entryLT).priority = 0;
					entryLT->op = LT::operations::EQUOPER;
					break;
				case NEQU:
					(*entryLT).priority = 0;
					entryLT->op = LT::operations::NEQUOPER;
					break;
				}
				LT::Add(lexTable, *entryLT);
				continue;
			}
			else if (FST::execute(FST::FST(word[i], FST_COMMA)))
			{
				LT::Entry* entryLT = new LT::Entry(LEX_COMMA, line, LT_TI_NULLIDX);
				(*entryLT).priority = 1;
				LT::Add(lexTable, *entryLT);
				continue;
			}
			else if (FST::execute(FST::FST(word[i], FST_SEMICOLON)))
			{
				if (findDeclaration && findFunc && findType)
					functions.pop();
				LT::Entry* entryLT = new LT::Entry(LEX_SEMICOLON, line, LT_TI_NULLIDX);
				LT::Add(lexTable, *entryLT);
				findDeclaration = findFunc = findType = findParm = false;
				entryLT = {};
				continue;
			}
			else if (FST::execute(FST::FST(word[i], FST_LEFTBRACE)))
			{
				findType = findDeclaration = findFunc = findParm = findProc = false;
				LT::Entry* entryLT = new LT::Entry(LEX_LEFTBRACE, line, LT_TI_NULLIDX);
				LT::Add(lexTable, *entryLT);
				continue;
			}
			else if (FST::execute(FST::FST(word[i], FST_BRACELET)))
			{
				if (!functions.empty() && is_cycle == 0)
					functions.pop();
				else if (is_cycle != 0) is_cycle--;
				LT::Entry* entryLT = new LT::Entry(LEX_BRACELET, line, LT_TI_NULLIDX);
				LT::Add(lexTable, *entryLT);
				continue;
			}
			else if (FST::execute(FST::FST(word[i], FST_LEFTTHESIS)))
			{
				LT::Entry* entryLT = new LT::Entry(LEX_LEFTHESIS, line, LT_TI_NULLIDX);
				(*entryLT).priority = 0;
				parmsCounter = 0;
				LT::Add(lexTable, *entryLT);
				if (findFunc || findProc)
					findParm = true;
				continue;
			}
			else if (FST::execute(FST::FST(word[i], FST_RIGHTTHESIS)))
			{
				LT::Entry* entryLT = new LT::Entry(LEX_RIGHTHESIS, line, LT_TI_NULLIDX);
				(*entryLT).priority = 0;
				if (findFunc)
				{
					int c = parmsCounter - 1;
					for (int k = idTable.size - 1; k >= idTable.size - parmsCounter; k--, c--)
					{
						idTable.table[NOWIDXFUNC].parm[c].idDataType = idTable.table[k].idDataType;
					}
					idTable.table[NOWIDXFUNC].countOfPar = parmsCounter;
				}
				findParm = false;
				LT::Add(lexTable, *entryLT);
				continue;
			}
			else if (FST::execute(FST::FST(word[i], FST_EQUAL)))
			{
				LT::Entry* entryLT = new LT::Entry(LEX_RAV, line, LT_TI_NULLIDX);
				LT::Add(lexTable, *entryLT);
				continue;
			}
			else if (word[i][0] == DIV) {
				line++;
				position = 0;  // обноружен | значит перешли на следущую строку
				indexLex--;
				continue;

			}
			std::cout << Error::geterror(205).message << ", строка: " << line << "\n";
			throw ERROR_THROW_IN(205, line, position);

		}
		if (!findMain)
		{
			std::cout << Error::geterror(302).message << "\n";
			throw ERROR_THROW(302);
		}

		if (mainCounter > 1)
		{
			std::cout << Error::geterror(301).message << "\n";
			throw ERROR_THROW(301);
		}
		lex.idtable = idTable;
		lex.lextable = lexTable;
		return lex;

	}

	void Synchronization(Lexis::LEX& lex)
	{
		bool* is_changed = new bool[lex.idtable.size] { false };
		for (int i = 0; i < lex.lextable.size; i++) {
			if (lex.lextable.table[i].idxTI != -1 && !is_changed[lex.lextable.table[i].idxTI]) {
				lex.idtable.table[lex.lextable.table[i].idxTI].idxFirstLine = i;
				is_changed[lex.lextable.table[i].idxTI] = true;
			}
		}
		delete[] is_changed;
	}

	void ShowTable(LT::LexTable& lextable, std::ostream& out)
	{
		out << std::endl << "Lexical Table" << std::endl;
		out << std::setfill('-') << std::setw(40) << '-' << std::endl;
		out << "№   " << " | " << "lex" << "  | " << "idx in IT" << " |" << "номер строки" << " |" << std::endl;
		out << std::setw(40) << '-' << std::endl;

		for (int i = 0; i < lextable.size; i++)
		{
			out << std::setfill('0') << std::setw(4) << std::right << i << " | ";
			out << std::setfill(' ') << std::setw(4) << std::left << lextable.table[i].lexema << " | ";
			// если -1 то пробелы
			if (lextable.table[i].idxTI != -1)
				out << std::setfill(' ') << std::setw(9) << std::left << lextable.table[i].idxTI << " | ";
			else
			{
				out << std::setfill(' ') << std::setw(9) << std::left << ' ' << " | ";
			}
			out << std::setfill(' ') << std::setw(11) << std::left << lextable.table[i].strNumber << " | ";
			out << std::endl;
		}
	}
}