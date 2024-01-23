#include <string>
#include "pch.h"
#include <iostream>
#include "SematicAnalisis.h"

namespace Semantic
{
	void SemAnalyze(Lexis::LEX lex, Log::LOG log)
	{
		*log.stream << "\n-----------Работа семантического анализатора-----------" << std::endl;
		bool isGood = true;
		bool hasWar = false;
		int j = 0;
		int x = 0;
		int c;
		std::vector<std::string> functions_id;
		std::vector<std::string> varibles_id;
		for (int i = 0; i < lex.lextable.size; i++)
		{
			switch (lex.lextable.table[i].lexema) {
			case LEX_RAV:
				j = i + 1;
				for (j; lex.lextable.table[j].lexema != LEX_SEMICOLON; j++) { // проверяем все, что после равно
					switch (lex.lextable.table[j].lexema) {
					case LEX_ID:
					case LEX_LITERAL:
						if (lex.lextable.table[j].idxTI != -1 && lex.idtable.table[lex.lextable.table[j].idxTI].idType == IT::F)  // возвращаемое значение функции
							
						
						{
							if (lex.idtable.table[lex.lextable.table[j].idxTI].idDataType != lex.idtable.table[lex.lextable.table[i - 1].idxTI].idDataType && !hasWar)
							{     c = static_cast<int>(lex.idtable.table[lex.lextable.table[j].idxTI].value.vint);
								std::cout << Error::geterror(312).message << ", строка: " << lex.lextable.table[j].strNumber << "\n";
								Log::WriteError(log, Error::geterrorin(312, lex.lextable.table[j].strNumber, -1));
								hasWar = true;
								isGood = false;
							}

						}
						else if (lex.idtable.table[lex.lextable.table[j].idxTI].idDataType != lex.idtable.table[lex.lextable.table[i - 1].idxTI].idDataType
							&& lex.idtable.table[lex.lextable.table[j].idxTI].idDataType != IT::NONE
							&& lex.idtable.table[lex.lextable.table[i - 1].idxTI].idDataType != IT::NONE && !hasWar)// соотвествие типов данных в выражении
						{
							std::cout << Error::geterror(312).message << ", строка: " << lex.lextable.table[j].strNumber << "\n";
							Log::WriteError(log, Error::geterrorin(312, lex.lextable.table[j].strNumber, -1));
							isGood = false;
							hasWar = true;
						}

						continue;


					case LEX_DIRSLASH:

								 if (lex.lextable.table[j + 1].lexema == LEX_LEFTHESIS) {
							j += 2;// Перешли внутрь скобки
							int operand = static_cast<int>(lex.idtable.table[lex.lextable.table[j].idxTI].value.vint);
							int operand2 = static_cast<int>(lex.idtable.table[lex.lextable.table[j + 2].idxTI].value.vint);
							if (operand - operand2 == 0) {
								std::cout << Error::geterror(311).message << ", строка: " << lex.lextable.table[j].strNumber << "\n";
								Log::WriteError(log, Error::geterrorin(311, lex.lextable.table[j].strNumber, -1));
								isGood = false;
							}
						}


						
						
						 else if (j < lex.lextable.size - 1 &&
							lex.lextable.table[j + 1].idxTI != -1 &&
							lex.idtable.table[lex.lextable.table[j + 1].idxTI].idDataType == IT::INT &&
							static_cast<int>(lex.idtable.table[lex.lextable.table[j + 1].idxTI].value.vint) == 0) {
							// Проверка деления на ноль
							std::cout << Error::geterror(311).message << ", строка: " << lex.lextable.table[j].strNumber << "\n";
							Log::WriteError(log, Error::geterrorin(311, lex.lextable.table[j].strNumber, -1));
							isGood = false;

						}

						
						

						continue;





					case LEX_LEFTHESIS:// пропустить функцию 
						if (lex.lextable.table[j - 1].idxTI != -1) {
							if (lex.idtable.table[lex.lextable.table[j - 1].idxTI].idType == IT::F)
							{
								do {
									j++;
								} while (lex.lextable.table[j].lexema != LEX_RIGHTHESIS);
							}
						}
						continue;
					}
				}
				continue;
			case LEX_SEMICOLON:
				hasWar = false; // для того, чтобы не печатало одну и ту же ошибку в строке
				continue;
			case LEX_DECLARE:
				if (std::count(varibles_id.begin(), varibles_id.end(), lex.idtable.table[lex.lextable.table[i + 2].idxTI].id)) {// проверка на переопределение переменной
					Log::WriteError(log, Error::geterrorin(307, lex.lextable.table[i + 1].strNumber, -1));
					std::cout << Error::geterror(317).message << ", строка: " << lex.lextable.table[i + 2].strNumber << "\n";
					isGood = false;
				}
				varibles_id.push_back(lex.idtable.table[lex.lextable.table[i + 2].idxTI].id);
				continue;
			case LEX_ID:

				if (lex.idtable.table[lex.lextable.table[i].idxTI].idType == IT::F)   // проверка параметров функции 
				{
					x = 0;
					for (int h = i + 1; lex.lextable.table[h].lexema != LEX_RIGHTHESIS; h++)
					{
						if (lex.lextable.table[h].idxTI != -1)
						{
							/*совпадение типов данных*/if (lex.idtable.table[lex.lextable.table[i].idxTI].parm[x].idDataType != lex.idtable.table[lex.lextable.table[h].idxTI].idDataType && !hasWar)
							{
								std::cout << Error::geterror(309).message << ", строка: " << lex.lextable.table[i].strNumber << "\n";
								Log::WriteError(log, Error::geterrorin(309, lex.lextable.table[i].strNumber, -1));
								isGood = false;
								hasWar = true;
							}
							x++;
						}
					}
					/*совпадение кол-ва parms*/if (x != lex.idtable.table[lex.lextable.table[i].idxTI].countOfPar && !hasWar)
					{
						std::cout << Error::geterror(309).message << ", строка: " << lex.lextable.table[i].strNumber << "\n";
						Log::WriteError(log, Error::geterrorin(309, lex.lextable.table[i].strNumber, -1));
						isGood = false;
						hasWar = true;
					}

				}
				if (lex.idtable.table[lex.lextable.table[i].idxTI].idDataType == IT::NONE) // необъявленный id
				{
					std::cout << Error::geterror(305).message << ", строка: " << lex.lextable.table[i].strNumber << "\n";
					Log::WriteError(log, Error::geterrorin(305, lex.lextable.table[i].strNumber, -1));
					isGood = false;
				}
				continue;
			case LEX_CYCLE:
			case LEX_IF: // проверка условий в цикле и условном операторе 

				if (lex.lextable.table[i + 3].lexema == LEX_RIGHTHESIS && lex.idtable.table[lex.lextable.table[i + 2].idxTI].idDataType != IT::BOOL)
				{
					std::cout << Error::geterror(316).message << ", строка: " << lex.lextable.table[i].strNumber << "\n";
					Log::WriteError(log, Error::geterrorin(316, lex.lextable.table[i].strNumber, -1));
					isGood = false;
				}
				else if (lex.idtable.table[lex.lextable.table[i + 3].idxTI].idType == IT::OP)// если в условии 2 значения и операция 
				{
					if (lex.lextable.table[i + 3].op != LT::LESSOPER && lex.lextable.table[i + 3].op != LT::MOREOPER
						&& lex.lextable.table[i + 3].op != LT::EQUOPER && lex.lextable.table[i + 3].op != LT::NEQUOPER) // проверка на правильный оператор
					{
						std::cout << Error::geterror(310).message << ", строка: " << lex.lextable.table[i].strNumber << "\n";
						Log::WriteError(log, Error::geterrorin(310, lex.lextable.table[i].strNumber, -1));
						isGood = false;
					}
					if (lex.idtable.table[lex.lextable.table[i + 2].idxTI].idDataType == IT::STR // если есть строчный тип данных, то ошибка
						|| lex.idtable.table[lex.lextable.table[i + 4].idxTI].idDataType == IT::STR)
					{
						std::cout << Error::geterror(304).message << ", строка: " << lex.lextable.table[i].strNumber << "\n";
						Log::WriteError(log, Error::geterrorin(304, lex.lextable.table[i].strNumber, -1));
						isGood = false;
					}
					if (lex.idtable.table[lex.lextable.table[i + 2].idxTI].idDataType
						!= lex.idtable.table[lex.lextable.table[i + 4].idxTI].idDataType) // если не одинаковые типы
					{
						std::cout << Error::geterror(315).message << ", строка: " << lex.lextable.table[i].strNumber << "\n";
						Log::WriteError(log, Error::geterrorin(315, lex.lextable.table[i].strNumber, -1));
						isGood = false;
					}
				}
				continue;

			case LEX_FUNCTION:// проверка на переопределение функции
				if (std::count(functions_id.begin(), functions_id.end(), lex.idtable.table[lex.lextable.table[i + 1].idxTI].id)) {
					Log::WriteError(log, Error::geterrorin(307, lex.lextable.table[i + 1].strNumber, -1));
					std::cout << Error::geterror(317).message << ", строка: " << lex.lextable.table[i + 1].strNumber << "\n";
					isGood = false;
				}
				functions_id.push_back(lex.idtable.table[lex.lextable.table[i + 1].idxTI].id);

				j = i + 1;
				int type = lex.idtable.table[lex.lextable.table[j].idxTI].idDataType;
				int functions = -1;
				do {
					j++;
					if (lex.lextable.table[j].lexema == LEX_LEFTBRACE)
						functions++;
					else if (lex.lextable.table[j].lexema == LEX_BRACELET)
						functions--;
				} while ((lex.lextable.table[j].lexema != LEX_RETURN) || functions != 0);
				if (lex.idtable.table[lex.lextable.table[j + 1].idxTI].idDataType != type) { // если неправильный тип данных 
					std::cout << Error::geterror(314).message << ", строка: " << lex.lextable.table[j].strNumber << "\n";
					Log::WriteError(log, Error::geterrorin(314, lex.lextable.table[j].strNumber, -1));
					isGood = false;
				}
			}
		}

		if (!isGood)
			throw ERROR_THROW(109);
		*log.stream << "\n-----------Работа семантического анализатора прошла без ошибок-----------" << std::endl;
	}

}