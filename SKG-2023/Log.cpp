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
		time_t rowtime;// ���������� ���� time_t
		tm* timeinfo;
		time(&rowtime);//������� ���������� ������� ����������� �������� ������� � ��������
		timeinfo = localtime(&rowtime);//������� ��������������� ������� �������� �������, ������������ ��� ��������, ����� ��������� timeptr �� time_t � ��������� tm.
		char temp[100];
		strftime(temp, sizeof(temp), "\n------------��������------------\n ----%d.%m.%y %T----\n ", timeinfo);
		*log.stream << temp;
	}
	void WriteParm(LOG log, Parm::PARM parm)
	{
		char in_text[PARM_MAX_SIZE], out_text[PARM_MAX_SIZE], log_text[PARM_MAX_SIZE];
		wcstombs(in_text, parm.in, PARM_MAX_SIZE);
		wcstombs(out_text, parm.out, PARM_MAX_SIZE);//����������� ������ ������������ ��������, ���������� ����������-���������� in,
		// � ��� ������������� ����������
		wcstombs(log_text, parm.log, PARM_MAX_SIZE);
		*log.stream << "\n ---- ��������� ---- \n\n-in: " << in_text
			<< "\n-out: " << out_text
			<< "\n-log: " << log_text << endl;

	}
	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "\n---- �������� ������ ----\n"
			<< "\n����� ��������: " << in.size
			<< "\n����� �����: " << in.lines
			<< "\n���������: " << in.ignor << endl;
	}

	void WriteError(LOG log, Error::ERROR Err)
	{
		if (log.stream)
		{

			*log.stream << "\n������ " << Err.id
				<< ": " << Err.message
				<< "\n������ " << Err.inext.line
				<< " ������� " << Err.inext.col << endl;
		}
		else
		{
			cout << "\n������ " << Err.id
				<< ": " << Err.message
				<< "\n������ " << Err.inext.line
				<< " ������� " << Err.inext.col << endl;

		}
	}
	void Close(LOG log)
	{
		log.stream->close();
		delete log.stream;
	}
}