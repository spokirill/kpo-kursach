#pragma once
#define PARM_IN L"-in:"	
#define PARM_OUT L"-out:"	
#define PARM_LOG L"-log:"	
#define PARM_MAX_SIZE 300	 
#define PARM_LOG_DEFAULT_EXT L".log"
#define PARM_OUT_DEFAULT_EXT L".out.asm"
#define PARM_LOGOUTFILE_DEFAULT_EXT L".logOut"
namespace Parm
{
	struct PARM
	{
		wchar_t in[PARM_MAX_SIZE];
		wchar_t out[PARM_MAX_SIZE];
		wchar_t log[PARM_MAX_SIZE];
		wchar_t logOut[PARM_MAX_SIZE];
	};

	PARM getparm(int argc, wchar_t* argv[]);
}
