#include "pch.h"
#include "In.h"
#include "Error.h"
#include "Parm.h"

using namespace std;

namespace In
{
	IN getin(Parm::PARM parm)
	{
		IN in;
		unsigned char letter;
		bool strliteral = false;
		int i = 0, position = 0;
		bool strCommit = false;
		in.size = 0;
		in.lines = 0;
		in.ignor = 0;
		in.text = new char[IN_MAX_LEN_TEXT];

		int lines = 0;
		int col = 0;

		char* text = new char[IN_MAX_LEN_TEXT];

		ifstream file(parm.in);
		if (file.fail())
		{
			std::cout << Error::geterror(102).message;
			throw ERROR_THROW(102);
		}


		while (in.size < IN_MAX_LEN_TEXT)
		{
			letter = file.get();
			if (file.eof()) break;

			if (letter == '\n')
				strCommit = false;

			if (strCommit)
				continue;
			if (letter == '\'' || letter == '\"') // проверка на литерал
				strliteral = !strliteral;


			switch (in.code[(int)letter])
			{
			case in.BR:
				in.lines++;
				position = 0;
				in.text[i] = '|';
				i++;
				in.size++;
				break;
			case in.T:
				in.text[i] = letter;
				i++;
				in.size++;
				position++;
				break;
			case in.I:
				in.ignor++;
				position++;
				break;
			case in.CMIT:
				if (!strliteral)
					strCommit = true;
				else
				{
					in.text[i] = letter;
					i++;
					in.size++;
					position++;
				}
				break;
			case in.F:
				throw ERROR_THROW_IN(200, in.lines + 1, position);
				//std::cout << Error::geterror(200).message;
			default:
				in.text[i] = in.code[(int)letter];
				i++;
				in.size++;
				position++;
			}
		}
		file.close();
		in.text[i] = '\0';
		return in;
	}
}