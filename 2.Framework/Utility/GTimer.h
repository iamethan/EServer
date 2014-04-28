#ifndef __G_TIMER_H__
#define __G_TIMER_H__

#ifdef WIN32

#include "sys\timeb.h"
#include <windows.h>

#elif  defined __linux

#include <sys/time.h>
#include <unistd.h>

#else

#error "Unknown platform !"  

#endif

#include <time.h>

#ifdef WIN32

class GTimer
{
public:
	GTimer( float = -1.0f );
	~GTimer();

	const bool Enable() const     { return beEnable; }
	void Enable( bool rhs ) { beEnable=rhs; }

	bool     OnTime()    const;
	void     Reset( bool DoTimeCorrection=false );
	float    Seconds()   const; // return time from reset in second
	float    Percent()   const  // return time gone in percent
	{ return fInterval > 0.0f ? Seconds() / fInterval : 0.0f; }
	void     Interval( float );
	float    Interval()  const    { return fInterval; }

protected:
	float          fLastTime;
	float          fInterval;
	bool           beEnable;
};

#elif defined __linux

//Unix version
class GTimer 
{
public:
	GTimer( float = -1.0f );
	~GTimer();

	bool Enable() const     { return beEnable; }
	void Enable( bool rhs ) { beEnable=rhs; }

	bool     OnTime()    const;
	void     Reset( bool DoTimeCorrection=false );
	float	 Seconds()   const; // return time from reset in second
	float    Percent()   const  // return time gone in percent
	{ return fInterval > 0.0f ? Seconds() / lInterval : 0.0f; }
	void     Interval( long );
	float     Interval()  const    { return fInterval; }

protected:

	float          m_fLastTimeFraction;
	long           m_lLastTime;

	float          fInterval;

	bool           beEnable;
};

#else

#error "Unknown platform !"  

#endif

#endif // __G_TIMER_H__
