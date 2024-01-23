#include "pch.h"
#include "Polish.h"
#include "LexAnalysis.h"
using namespace std;

namespace Polish
{
	bool PolishNotation(int i, Lexis::LEX& lex)
	{
		stack<LT::Entry> stack;
		queue<LT::Entry> queue;

		LT::Entry aggrSymbol; // параметр
		aggrSymbol.idxTI = -1;
		aggrSymbol.lexema = '#';
		aggrSymbol.strNumber = lex.lextable.table[i].strNumber;

		LT::Entry funcSymbol; // функция 
		funcSymbol.idxTI = LT_TI_NULLIDX;
		funcSymbol.lexema = '@';
		funcSymbol.strNumber = lex.lextable.table[i].strNumber;

		int idx;

		int lexemCounter = 0;
		int parmCounter = 0;
		int lexemPosition = i;
		char* buf = new char[i];

		bool isFunction = false;
		for (i; lex.lextable.table[i].lexema != LEX_SEMICOLON; i++, lexemCounter++) {
			switch (lex.lextable.table[i].lexema) {
			case LEX_ID:
			case LEX_LITERAL:
				if (lex.idtable.table[lex.lextable.table[i].idxTI].idType == IT::F) {
					isFunction = true;
					idx = lex.lextable.table[i].idxTI;
				}
				else {
					if (isFunction)
						parmCounter++;
					queue.push(lex.lextable.table[i]);
				}
				continue;

			case LEX_LEFTHESIS:
				stack.push(lex.lextable.table[i]);
				continue;

			case LEX_RIGHTHESIS: // если ')' закидываем все из стека в результирующую и чистим стек 
				while (stack.top().lexema != LEX_LEFTHESIS) {
					queue.push(stack.top());
					stack.pop();
					if (stack.empty())
						return false;
				}

				if (!isFunction)
					stack.pop();
				else { // если функция 
					funcSymbol.idxTI = idx;
					idx = LT_TI_NULLIDX;
					lex.lextable.table[i] = funcSymbol;
					queue.push(lex.lextable.table[i]);
					_itoa_s(parmCounter, buf, 2, 10);
					stack.top().lexema = buf[0];
					stack.top().idxTI = LT_TI_NULLIDX;
					stack.top().strNumber = funcSymbol.strNumber;
					queue.push(stack.top());
					stack.pop();
					parmCounter = 0;
					isFunction = false;
				}
				continue;

			case LEX_OPERATOR:
				while (!stack.empty() && lex.lextable.table[i].priority <= stack.top().priority) { // пока приоритет меньше или равен последнему
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lex.lextable.table[i]);
				continue;
			}
		}

		while (!stack.empty()) {
			if (stack.top().lexema == LEX_LEFTHESIS || stack.top().lexema == LEX_RIGHTHESIS)
				return false;

			queue.push(stack.top());
			stack.pop();
		}

		while (lexemCounter != 0) {
			if (!queue.empty()) {
				lex.lextable.table[lexemPosition++] = queue.front();
				queue.pop();
			}
			else
				lex.lextable.table[lexemPosition++] = aggrSymbol;

			lexemCounter--;
		}

		for (int i = 0; i < lexemPosition; i++) {
			if (lex.lextable.table[i].lexema == LEX_OPERATOR || lex.lextable.table[i].lexema == LEX_LITERAL)
				lex.idtable.table[lex.lextable.table[i].idxTI].idxFirstLine = i;
		}

		return true;
	}

	void StartPolish(Lexis::LEX& lex)
	{
		for (int i = 0; i < lex.lextable.size; i++)
		{
			if (lex.lextable.table[i].lexema == '=')
				PolishNotation(i + 1, lex);
		}
	}
}