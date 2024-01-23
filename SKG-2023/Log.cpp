#include "pch.h"
#include "Log.h"	
#include <time.h>	
#include "IT.h"
#pragma warning(disable:4996)
using namespace std;
using namespace IT;
namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (log.stream->fail())
		{
			throw ERROR_THROW(112);
		}
		wcscpy_s(log.logfile, logfile);
		return log;
	}
	void WriteLine(LOG log, const char* c, ...)
	{
		const char** p = &c;
		int i = 0;
		while (p[i] != "")
		{
			*log.stream << *p;
			p++;

		}
		*log.stream << endl;
	}
	void WriteLine(LOG log, const wchar_t* c, ...)
	{
		const wchar_t** w_p = &c;
		int i = 0; char conv_arr[100];
		while (w_p[i] != L"")
		{
			wcstombs(conv_arr, w_p[i++], sizeof(conv_arr));
			*log.stream << conv_arr;
		}
		*log.stream << endl;
	}
	void WriteLog(LOG log)
	{
		time_t rowtime;// переменная типа time_t
		tm* timeinfo;
		time(&rowtime);//Функция возвращает текущее календарное значение времени в секундах
		timeinfo = localtime(&rowtime);//Функция преобразовывает текущее значение времени, передаваемое как аргумент, через указатель timeptr на time_t в структуру tm.
		char temp[100];
		strftime(temp, sizeof(temp), "\n------------ПРОТОКОЛ------------\n ----%d.%m.%y %T----\n ", timeinfo);
		*log.stream << temp;
	}
	void WriteParm(LOG log, Parm::PARM parm)
	{
		char in_text[PARM_MAX_SIZE], out_text[PARM_MAX_SIZE], log_text[PARM_MAX_SIZE];
		wcstombs(in_text, parm.in, PARM_MAX_SIZE);
		wcstombs(out_text, parm.out, PARM_MAX_SIZE);//преобразует массив двухбайтовых символов, адресуемый параметром-указателем in,
		// в его многобайтовый эквивалент
		wcstombs(log_text, parm.log, PARM_MAX_SIZE);
		*log.stream << "\n ---- Параметры ---- \n\n-in: " << in_text
			<< "\n-out: " << out_text
			<< "\n-log: " << log_text << endl;

	}
	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "\n---- Исходные данные ----\n"
			<< "\nВсего символов: " << in.size
			<< "\nВсего строк: " << in.lines
			<< "\nПропущено: " << in.ignor << endl;
	}

	void WriteError(LOG log, Error::ERROR Err)
	{
		if (log.stream)
		{

			*log.stream << "\nОшибка " << Err.id
				<< ": " << Err.message
				<< "\nСтрока " << Err.inext.line
				<< " позиция " << Err.inext.col << endl;
		}
		else
		{
			cout << "\nОшибка " << Err.id
				<< ": " << Err.message
				<< "\nСтрока " << Err.inext.line
				<< " позиция " << Err.inext.col << endl;

		}
	}
	void Close(LOG log)
	{
		log.stream->close();
		delete log.stream;
	}
}