#include "GTimer.h"

#ifdef WIN32

GTimer::GTimer(  float f ) : 
fInterval(f), beEnable(true)
{
	Reset();
}

GTimer::~GTimer() 
{
}

bool GTimer::OnTime() const 
{
	if( !beEnable )      return false;
	if( fInterval < 0 )  return false;
	if( fInterval == 0 ) return true;

	if( Seconds() < fInterval ) 
		return false;
	else
		return true;
}

float GTimer::Seconds() const 
{
	float fSec = ::timeGetTime()/1000.0f - fLastTime;

	return fSec;
}

void GTimer::Reset( bool beCorrecting  )
{
	if( beCorrecting )
		fLastTime+= fInterval;
	else
		fLastTime = ::timeGetTime() / 1000.0f;
}

void GTimer::Interval( float rhs ) 
{
	fInterval = rhs;
}

#else 

//linux version
GTimer::GTimer( float f ):
fInterval(f), beEnable(true)
{
	Reset();
}

GTimer::~GTimer() 
{
}

bool   GTimer::OnTime() const 
{
	if( !beEnable )      return false;
	if( fInterval < 0 )  return false;
	if( fInterval == 0 ) return true;

	if( Seconds() < fInterval ) 
		return false;
	else
		return true;
}

float  GTimer::Seconds() const 
{
	timeval tv;
	gettimeofday( &tv, NULL );

	long  lTime = tv.tv_sec;
	float fTimeFraction = ((float)tv.tv_usec)/1000000.0f;

	return (lTime - m_lLastTime) + (fTimeFraction  - m_fLastTimeFraction);
}

void  GTimer::Reset( bool beCorrecting  ) 
{
	if( beCorrecting ) {
		m_lLastTime += (long) fInterval;
		m_fLastTimeFraction += ( fInterval - (long) fInterval );
	}
	else {
		timeval tv;
		gettimeofday( &tv, NULL );
		m_fLastTimeFraction = (float)tv.tv_usec/1000000.0f;
		m_lLastTime = tv.tv_sec;
	}
}

void GTimer::Interval( float rhs ) 
{
	fInterval = rhs;
}

#endif