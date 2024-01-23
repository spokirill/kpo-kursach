#include "pch.h"
#include "Div.h"
#include "Error.h"
namespace Div
{
	void cleanup(char source[], int size, Log::LOG logfile) {
		char Separators[] = { " ,;(){}=+-*/|%" };
		bool findLiteral = false;
		int count = NULL;

		for (int i = 0; i < size; i++) {
			if (source[i] == '\"') {
				findLiteral = !findLiteral;
				count++;
			}

			if ((source[i] == ' ' || source[i] == '\t') && !findLiteral) {
				for (int j = 0; j < sizeof(Separators) - 1; j++) {
					if (source[i + 1] == Separators[j] || source[i - 1] == Separators[j] || i == 0) {
						for (int k = i; k < size; k++) {
							source[k] = source[k + 1];
						}
						i--;
						break;
					}
				}
			}
		}

		if (count % 2 != 0)
		{
			std::cout << Error::geterror(300).message << "\n";
			throw ERROR_THROW(300);
		}
	}
	char** DivideText(char source[], int size)
	{
		char** word = new char* [maxWord];
		for (int i = 0; i < maxWord; i++) {
			word[i] = new char[sizeWord] {NULL};
		}

		bool findSeparator, findLiteral = false;
		int j = 0;
		char Separators[] = { " ,;(){}=+-*/|%" };
		for (int i = 0, k = 0; i < size - 1; i++, k++) {
			findSeparator = false;
			if (source[i] == '\"' || source[i] == '\'') findLiteral = !findLiteral;
			for (int t = 0; t < sizeof(Separators) - 1; t++) {
				if (source[i] == Separators[t] && !findLiteral) {
					findSeparator = true;
					if (word[j][0] != NULL) {
						word[j++][k] = '\0';
						k = 0;
					}
					if (Separators[t] == ' ') {
						k = -1;
						break;
					}
					word[j][k++] = Separators[t];
					word[j++][k] = '\0';
					k = -1;
					break;
				}
			}

			if (!findSeparator) word[j][k] = source[i];
		}

		word[j] = NULL;
		for (int i = 0; i < j; i++) {
			if (!strcmp((char*)word[i], "")) return NULL;
		}

		return word;
	}
}