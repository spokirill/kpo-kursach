#include "pch.h"
#include"Parm.h"
#include "Error.h"

namespace Parm
{
	PARM getparm(int argc, wchar_t* argv[])
	{
		PARM p;
		bool in = false, out = false, log = false;

		for (int i = 1; i < argc; i++)
		{
			if (wcslen(argv[i]) > PARM_MAX_SIZE) throw ERROR_THROW(104);

			if (wcsstr(argv[i], PARM_IN))//находит первое вхождение источника в строку назначения
			{
				wcscpy_s(p.in, argv[i] + wcslen(PARM_IN));
				in = true;
			}
			else if (wcsstr(argv[i], PARM_OUT))
			{
				wcscpy_s(p.out, argv[i] + wcslen(PARM_OUT));
				out = true;
			}
			else if (wcsstr(argv[i], PARM_LOG))
			{
				wcscpy_s(p.log, argv[i] + wcslen(PARM_LOG));
				log = true;
			}


		}
		if (!in) throw ERROR_THROW(100);

		if (!out)
		{
			wcscpy_s(p.out, p.in);
			wcscat_s(p.out, PARM_OUT_DEFAULT_EXT);
		}
		if (!log)
		{
			wcscpy_s(p.log, p.in);
			wcscat_s(p.log, PARM_LOG_DEFAULT_EXT);
		}
		wcscpy_s(p.logOut, p.in);
		wcscat_s(p.logOut, PARM_LOGOUTFILE_DEFAULT_EXT);
		return p;
	}


}