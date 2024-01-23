#include "pch.h"
#include "Generation.h"

namespace Gen
{
	Generator::Generator(LT::LexTable plexT, IT::IdTable pidT, wchar_t pout[])
	{
		lexT = plexT;
		idT = pidT;
		out = std::ofstream(pout, std::ios_base::out);

		Head();
		Const();
		Data();
		Code();
		out.close();
	}

	void Generator::Head()  // подключение lib и прототипы функций 
	{
		out << ".586\n";
		out << ".model flat, stdcall\n";
		out << "includelib libucrt.lib\n";
		out << "includelib kernel32.lib\n";
		out << "includelib C:\\Users\\givi\\Documents\\kursach\\SKG-2023\\SKG-2023\\Debug\\Staticlib.lib\n";
		out << "ExitProcess proto :dword\n\n";
		out << "EXTRN BREAKL: proc\n";
		out << "EXTRN OutputInt: proc\n";
		out << "EXTRN OutputChar: proc\n";
		out << "EXTRN OutputBool: proc\n";
		out << "\n.stack 4096\n\n";
	}

	void Generator::Const() // сегмент констант 
	{
		out << ".const\n";
		for (int i = 0; i < idT.size; i++)
		{
			if (idT.table[i].idType == IT::L)
			{
				out << "\t" << idT.table[i].id;
				if (idT.table[i].idDataType == IT::STR)
					out << " byte \"" << idT.table[i].value.vstr.str << "\",0";
				else if (idT.table[i].idDataType == IT::INT || idT.table[i].idDataType == IT::BOOL)
					out << " sdword " << idT.table[i].value.vint;
				else if (idT.table[i].idDataType == IT::CHAR)
					out << " word \'" << idT.table[i].value.vchar << "\'";
				out << "\n";
			}
		}

	}

	void Generator::Data() // сегмент данных 
	{
		out << "\n.data\n";
		out << "\tbuffer BYTE 256 dup(0)\n";
		for (int i = 0; i < lexT.size; i++)
		{
			if (lexT.table[i].lexema == LEX_DECLARE && idT.table[lexT.table[i + 2].idxTI].idType == IT::V)
			{
				out << "\t" << idT.table[lexT.table[i + 2].idxTI].id;
				switch (idT.table[lexT.table[i + 2].idxTI].idDataType)
				{
				case IT::STR:
					out << " dword ?\n";
					break;
				case IT::INT:
					out << " sdword 0\n";
					break;
				case IT::BOOL:
					out << " word 0\n";
					break;
				case IT::CHAR:
					out << " word ?\n";
					break;
				default:
					break;
				}
			}
		}
	}
	void Generator::Code() // код
	{
		bool strdlina = false;
		bool hasoper = false;
		std::stack<char*> stack;
		std::stack<char*> idStack;
		int numOfRet = 0,
			numOfCycles = 0,
			numOfPoints = 0,
			numOfEnds = 0,
			countParms = 0;

		std::string strRet = std::string(), // код recive
			cycleCode = std::string(), // код цикла
			funcName = std::string();

		bool flagFunc = false,
			flagRet = false,
			flagBody = false,
			flagIf = false,
			flag_then = false,
			flagElse = false,
			flagProc = false,
			flagCallfunc = false,
			flagCondition = false,
			flagCycle = false;

		int resultPosition;

		out << "\n.code\n";
		for (int i = 0; i < lexT.size; i++)
		{
			switch (lexT.table[i].lexema)
			{
			case LEX_PROCEDURE:
			case LEX_FUNCTION:
				while (lexT.table[i].lexema != LEX_RIGHTHESIS) {
					if (lexT.table[i].lexema == LEX_ID && idT.table[lexT.table[i].idxTI].idType == IT::F) {
						if (idT.table[lexT.table[i].idxTI].idDataType == IT::PROC)
							flagProc = true;
						out << (funcName = std::string((char*)idT.table[lexT.table[i].idxTI].id)) << " proc ";
					}
					else if (lexT.table[i].lexema == LEX_ID && idT.table[lexT.table[i].idxTI].idType == IT::P) {
						out << idT.table[lexT.table[i].idxTI].id << " : ";
						if (idT.table[lexT.table[i].idxTI].idDataType == IT::INT)
							out << "sdword";
						else if (idT.table[lexT.table[i].idxTI].idDataType == IT::BOOL)
							out << "word";
						else if (idT.table[lexT.table[i].idxTI].idDataType == IT::CHAR)
							out << "word";
						else
							out << "dword";
					}
					else if (lexT.table[i].lexema == LEX_COMMA)
						out << ", ";

					i++;
				}
				flagFunc = true;
				out << "\n";
				break;
			case LEX_ID:
				if (idT.table[lexT.table[i].idxTI].idType == IT::F) // если идентификатор функция 
				{
					int count = 0;
					for (int j = i + 2; lexT.table[j].lexema; j++)
					{
						if (lexT.table[j].lexema == LEX_RIGHTHESIS) break;
						else if (lexT.table[j].lexema == LEX_COMMA) j++;
						// формируем стек для функции 
						if (idT.table[lexT.table[j].idxTI].idDataType == IT::INT || idT.table[lexT.table[j].idxTI].idDataType == IT::BOOL || idT.table[lexT.table[j].idxTI].idDataType == IT::CHAR)
						{
							count++;
							idStack.push(idT.table[lexT.table[j].idxTI].id);
						}
						else if (idT.table[lexT.table[j].idxTI].idDataType == IT::STR) { // стринговый парметр 
							count++;

							char* s;
							if (idT.table[lexT.table[j].idxTI].idType == IT::L) { // литерал  с offset
								s = new char[8] { "offset " };
							}
							else { // переменная 
								s = new char[1] { "" };
							}

							size_t len1 = strlen((char*)s);
							size_t len2 = strlen((char*)idT.table[lexT.table[j].idxTI].id);
							char* result = (char*)malloc(len1 + len2 + 1);
							memcpy(result, s, len1);
							memcpy(result + len1, (char*)idT.table[lexT.table[j].idxTI].id, len2 + 1);
							idStack.push(result);
						}
					}
					for (int k = 0; k < count; k++) { //  пушим параметры в стек 
						out << "\tpush " << idStack.top() << "\n";
						idStack.pop();
					}
					out << "\tcall " << idT.table[lexT.table[i].idxTI].id << "\n"; // вызов функции 
				}
				break;

			case LEX_MAINFUNC:
				flagBody = true;
				out << "main proc\n";
				break;
			case LEX_RAV: // выражение 
				resultPosition = i - 1;
				while (lexT.table[i].lexema != LEX_SEMICOLON) {
					switch (lexT.table[i].lexema) {
					case LEX_ID:
					case LEX_LITERAL:
						strdlina = false;
						if (idT.table[lexT.table[i].idxTI].idType == IT::F)
							flagCallfunc = true;
						if (idT.table[lexT.table[i].idxTI].idDataType == IT::INT || idT.table[lexT.table[i].idxTI].idDataType == IT::BOOL || idT.table[lexT.table[i].idxTI].idDataType == IT::CHAR) // 
						{
							out << "\tpush " << idT.table[lexT.table[i].idxTI].id << "\n"; // пушить в стек переменнную или литерал 
							stack.push(idT.table[lexT.table[i].idxTI].id);
							break;
						}
						else if (idT.table[lexT.table[i].idxTI].idDataType == IT::STR)
						{
							char* s;
							if (idT.table[lexT.table[i].idxTI].idType == IT::L) {
								out << "\tpush offset " << idT.table[lexT.table[i].idxTI].id << "\n";
								s = new char[8] { "offset " };
							}
							else {
								out << "\tpush " << idT.table[lexT.table[i].idxTI].id << "\n";
								s = new char[1] { "" };
							}

							size_t len1 = strlen((char*)s);
							size_t len2 = strlen((char*)idT.table[lexT.table[i].idxTI].id);
							char* result = (char*)malloc(len1 + len2 + 1);
							memcpy(result, s, len1);
							memcpy(result + len1, (char*)idT.table[lexT.table[i].idxTI].id, len2 + 1);
							stack.push(result);
							break;
						}

					case LEX_OPERATOR:

						if (strdlina)
						{
							strdlina = false;
							switch (lexT.table[i].op) {
							case LT::MULOPER:
								out << "\tpop eax\n\tpop ebx\n\tpop ebx\n";
								out << "\tmul ebx\n\tpush eax\n";
								break;
							case LT::PLUSOPER:
								out << "\tpop eax\n\tpop ebx\n\tpop ebx\n";
								out << "\tadd eax, ebx\n\tpush eax\n";
								break;
							case LT::MINUSOPER:
								out << "\tpop ebx\n\tpop eax\n\tpop eax\n";
								out << "\tsub eax, ebx\n\tpush eax\n";
								break;
							case LT::DIVOPER:
								out << "\tpop ebx\n\tpop eax\n\tpop eax\n";
								out << "\tcmp ebx, 0\n";
								out << "\tje divzero\n";
								out << "\tcdq\n\tidiv ebx\n\tpush eax\n";
								break;
							case LT::DIVMODOPER:
								out << "\tpop ebx\n\tpop eax\n\tpop eax\n";
								out << "\tcmp ebx, 0\n";
								out << "\tje divzero\n";
								out << "\tcdq\n\tidiv ebx\n\tpush edx\n";
								break;
							}
							break;
						}
						else
						{
							switch (lexT.table[i].op) { // арифметические операции 
							case LT::MULOPER:
								out << "\tpop eax\n\tpop ebx\n";
								out << "\tmul ebx\n\tpush eax\n";
								break;
							case LT::PLUSOPER:
								out << "\tpop eax\n\tpop ebx\n";
								out << "\tadd eax, ebx\n\tpush eax\n";
								break;
							case LT::MINUSOPER:
								out << "\tpop ebx\n\tpop eax\n";
								out << "\tsub eax, ebx\n\tpush eax\n";
								break;
							case LT::DIVOPER:
								out << "\tpop ebx\n\tpop eax\n";
								out << "\tcmp ebx, 0\n";
								out << "\tje divzero\n";
								out << "\tcdq\n\tidiv ebx\n\tpush eax\n";
								break;
							case LT::DIVMODOPER:
								out << "\tpop ebx\n\tpop eax\n";
								out << "\tcmp ebx, 0\n";
								out << "\tje divzero\n";
								out << "\tcdq\n\tidiv ebx\n\tpush edx\n";
								break;
							}
							break;
						}
					case '@': // параметры  // вызов функции 
						countParms = (char)lexT.table[i + 1].lexema - '0';

						for (int j = 1; j <= countParms; j++)
							out << "\tpop edx\n";

						for (int j = 1; j <= countParms; j++) {
							out << "\tpush " << stack.top() << "\n";
							stack.pop();
						}
						if (strcmp(idT.table[lexT.table[i].idxTI].id, "strlength") == 0 || strcmp(idT.table[lexT.table[i].idxTI].id, "stoi") == 0)
						{
							strdlina = true;
						}

						out << "\tcall " << idT.table[lexT.table[i].idxTI].id << "\n\tpush eax\n";
						flagCallfunc = false;
						break;
					}

					i++;
				}

				out << "\tpop " << idT.table[lexT.table[resultPosition].idxTI].id << "\n";
				break;
			case '@':// параметры  // вызов функции 
				countParms = (char)lexT.table[i + 1].lexema - '0';
				for (int j = 1; j <= countParms; j++) {
					if (idT.table[lexT.table[i - j].idxTI].idDataType == IT::INT || idT.table[lexT.table[i - j].idxTI].idDataType == IT::BOOL)
						out << "\tpush " << idT.table[lexT.table[i - j].idxTI].id << "\n";
					else if (idT.table[lexT.table[i].idxTI].idDataType == IT::CHAR)
					{
						out << "\tpush  " << idT.table[lexT.table[i - j].idxTI].id << "\n";
					}
					else {
						if (idT.table[lexT.table[i - j].idxTI].idType == IT::L)
							out << "\tpush offset " << idT.table[lexT.table[i - j].idxTI].id << "\n";
						else
							out << "\tpush " << idT.table[lexT.table[i - j].idxTI].id << "\n";
					}
				}

				out << "\tcall " << idT.table[lexT.table[i - countParms - 1].idxTI].id << "\n";
				break;

			case LEX_RETURN: // recive прыгнуть по метке где pop eax
				out << "\tpush ";
				if (idT.table[lexT.table[i + 1].idxTI].idType == IT::L)
					out << idT.table[lexT.table[i + 1].idxTI].value.vint << "\n";
				else
					out << idT.table[lexT.table[i + 1].idxTI].id << "\n";
				if (flagFunc) {
					out << "\tjmp local" << numOfRet << "\n";
					flagRet = true;
				}
				if (flagBody) {
					out << "\t\tjmp theend\n";
					flagRet = true;
				}
				break;
			case LEX_BRACELET:
				if (flagBody && !flag_then && !flagElse && !flagFunc && !flagCycle) {
					if (flagRet) {
						out << "theend:\n";
						flagRet = false;
					}
					out << "\tcall ExitProcess\ndivzero:\ncall ExitProcess\nmain ENDP\nend main";
				}
				if (flagFunc && !flag_then && !flagElse && !flagCycle) {
					if (flagRet) {
						out << "local" << numOfRet++ << ":\n";
						out << "\tpop eax\n\tret\n";
						flagRet = false;
					}
					if (flagProc) {
						out << "\tret\n";
						flagProc = false;
					}
					out << "\ndivzero:\ncall ExitProcess\n";
					out << funcName << " ENDP\n\n";
					flagFunc = false;
				}
				if (flag_then) { // 
					flag_then = false;
					if (flagElse) {
						out << "\tjmp e" << numOfEnds << "\n";
						flagElse = false;
					}
					out << "m" << numOfPoints++ << ":\n";
				}
				if (flagElse) {
					flagElse = false;
					out << "e" << numOfEnds++ << ":\n";
				}
				if (flagCycle) {
					out << cycleCode << "cyclenext" << numOfCycles << ":\n";
					cycleCode.clear();
					numOfCycles++;
					flagCycle = false;
				}
				break;
			case LEX_IF:
				flagIf = true;
				break;

			case LEX_CYCLE:
				flagCycle = true;
				flagCondition = true;
				break;

			case LEX_ELSE:
				flagElse = true;
				break;

			case LEX_LEFTHESIS:
				if (flagIf) { // условие condition
					if (idT.table[lexT.table[i + 1].idxTI].idDataType == IT::BOOL && lexT.table[i + 2].lexema == LEX_RIGHTHESIS) {
						out << "\tmov eax, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";
						out << "\tcmp eax, 1\n";
						out << "\tjz m" << numOfPoints << "\n";
						out << "\tjnz m" << numOfPoints + 1 << "\n";
					}
					else {
						if (idT.table[lexT.table[i + 1].idxTI].idDataType == IT::BOOL || idT.table[lexT.table[i + 1].idxTI].idDataType == IT::CHAR)
						{
							out << "\tmovzx eax, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";
							out << "\tmovzx ebx, " << idT.table[lexT.table[i + 3].idxTI].id << "\n";
							out << "\tcmp eax, ebx\n";
						}
						else
						{
							out << "\tmov eax, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";
							out << "\tcmp eax, " << idT.table[lexT.table[i + 3].idxTI].id << "\n";
						}
						switch (lexT.table[i + 2].op)
						{
						case LT::MOREOPER:
							out << "\tjg m" << numOfPoints << "\n";
							out << "\tjl m" << numOfPoints + 1 << "\n";
							break;
						case LT::LESSOPER:
							out << "\tjl m" << numOfPoints << "\n";
							out << "\tjg m" << numOfPoints + 1 << "\n";
							break;
						case LT::EQUOPER:
							out << "\tjz m" << numOfPoints << "\n";
							out << "\tjnz m" << numOfPoints + 1 << "\n";
							break;
						case LT::NEQUOPER:
							out << "\tjnz m" << numOfPoints << "\n";
							out << "\tjz m" << numOfPoints + 1 << "\n";
							break;
						}
					}
					out << "\tje m" << numOfPoints + 1 << "\n";
					int j = i;
					while (lexT.table[j++].lexema != LEX_BRACELET) {
						if (lexT.table[j + 1].lexema == LEX_ELSE) {
							flagElse = true;
							break;
						}
					}
				}

				if (flagCondition) { // если условие цикла
					if (idT.table[lexT.table[i + 1].idxTI].idDataType == IT::BOOL && lexT.table[i + 2].lexema == LEX_RIGHTHESIS) { // если один параметр
						out << "\tmov eax, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";
						out << "\tcmp eax, 1\n";
						cycleCode = "\tmov eax, " + std::string((char*)idT.table[lexT.table[i + 1].idxTI].id) + "\n" +
							"\tcmp eax, " + std::string((char*)idT.table[lexT.table[i + 1].idxTI].value.vint) + "\n";
						cycleCode += "\tjz cycle" + std::to_string(numOfCycles) + "\n";
						out << "\tjz cycle" << numOfCycles << "\n";
					}
					else { // если 2 параметра 
						cycleCode = "\tmov eax, " + std::string((char*)idT.table[lexT.table[i + 1].idxTI].id) + "\n" +
							"\tcmp eax, " + std::string((char*)idT.table[lexT.table[i + 3].idxTI].id) + "\n";
						out << "\tmov eax, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";
						out << "\tcmp eax, " << idT.table[lexT.table[i + 3].idxTI].id << "\n";
						switch (lexT.table[i + 2].op)
						{
						case LT::MOREOPER:
							cycleCode += "\tjg cycle" + std::to_string(numOfCycles) + "\n";
							out << "\tjg cycle" << numOfCycles << "\n";
							break;
						case LT::LESSOPER:
							cycleCode += "\tjl cycle" + std::to_string(numOfCycles) + "\n";
							out << "\tjl cycle" << numOfCycles << "\n";
							break;
						case LT::EQUOPER:
							cycleCode += "\tjz cycle" + std::to_string(numOfCycles) + "\n";
							out << "\tjz cycle" << numOfCycles << "\n";
							break;
						case LT::NEQUOPER:
							cycleCode += "\tjnz cycle" + std::to_string(numOfCycles) + "\n";
							out << "\tjnz cycle" << numOfCycles << "\n";
							break;
						}
					}
					out << "\tjmp cyclenext" << numOfCycles << "\n";
				}
				break;
			case LEX_RIGHTHESIS:
				if (lexT.table[i + 1].lexema == LEX_LEFTBRACE && flagIf) {
					flag_then = true;
					out << "m" << numOfPoints++ << ":\n";
					flagIf = false;
				}
				if (lexT.table[i + 1].lexema == LEX_LEFTBRACE && flagCondition) {
					out << "cycle" << numOfCycles << ":\n";
					flagCondition = false;
				}
				break;
			case LEX_WRITE: // печатать 
				if (idT.table[lexT.table[i + 1].idxTI].idDataType == IT::INT)
					out << "\tpush " << idT.table[lexT.table[i + 1].idxTI].id << "\n\tcall OutputInt\n";
				else if (idT.table[lexT.table[i + 1].idxTI].idDataType == IT::BOOL)
					out << "\tpush " << idT.table[lexT.table[i + 1].idxTI].id << "\n\tcall OutputBool\n";
				else if (idT.table[lexT.table[i + 1].idxTI].idDataType == IT::CHAR)
					out << "\tpush " << idT.table[lexT.table[i + 1].idxTI].id << "\n\tcall OutputChar\n";
				else {
					if (idT.table[lexT.table[i + 1].idxTI].idType == IT::L)
						out << "\tpush offset ";
					else
						out << "\tpush ";
					out << idT.table[lexT.table[i + 1].idxTI].id << "\n\tcall OutputStr\n";
				}
				break;
			case LEX_BREAKL:
				out << "\tcall BREAKL\n";
				break;
			}

		}
	}
}



