#include "GameUtil.h"

#if defined(WIN32)
#include <windows.h>
#elif defined(__LINUX__)
#include <unistd.h>
#endif

void MySleep(unsigned int millionseconds )
{
#if defined(WIN32)
	Sleep(millionseconds) ;
#elif defined(__LINUX__)
	usleep( millionseconds*1000 ) ;
#endif
}
