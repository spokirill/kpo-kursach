#include "pch.h"
#include "Out.h"
#include "Error.h"
using namespace std;
namespace OUT
{
	OUT getout(wchar_t outFile[])
	{
		OUT out;
		out.stream = new ofstream;
		out.stream->open(outFile);
		if (out.stream->fail())
		{
			throw ERROR_THROW(113);
		}
		return out;
	}
	void Close(OUT out)
	{
		out.stream->close();
		delete out.stream;
	}
}