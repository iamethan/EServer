#include "TimeManager.h"
#include <assert.h>
#include <math.h>

#if defined(WIN32)
#include<windows.h>
#endif

unsigned int g_FileNameFix_Last = 0 ;
int g_FileNameFix = 0;

TimeManager g_TimeManager;

TimeManager::TimeManager( )
{
	m_CurrentTime = 0 ;
}

TimeManager::~TimeManager( )
{

}

bool TimeManager::Init( )
{

#if defined(WIN32)
	m_StartTime = GetTickCount() ;
	m_CurrentTime = GetTickCount() ;
#elif defined(__LINUX__)
		m_StartTime		= 0;
	m_CurrentTime	= 0;
	gettimeofday(&_tstart, &tz);
#endif
	SetTime( ) ;

	g_FileNameFix = Time2Day( ) ;
	g_FileNameFix_Last = CurrentDate( ) ;

	return true ;
}

unsigned int TimeManager::CurrentTime( )
{
#if defined(WIN32)
		m_CurrentTime = GetTickCount() ;
#elif defined(__LINUX__)
		gettimeofday(&_tend,&tz);
	double t1, t2;
	t1 =  (double)_tstart.tv_sec*1000 + (double)_tstart.tv_usec/1000;
	t2 =  (double)_tend.tv_sec*1000 + (double)_tend.tv_usec/1000;
	m_CurrentTime = (unsigned int)(t2-t1);
#endif

	return m_CurrentTime ;
}

unsigned int TimeManager::CurrentDate()
{
	SetTime( ) ;
	unsigned int Date;
	ConvertTU(&m_TM,Date);

	return Date;
}


void TimeManager::SetTime( )
{
	time( &m_SetTime ) ;
	tm* ptm = localtime( &m_SetTime ) ;
	m_TM = *ptm ;
}

// 得到标准时间
time_t TimeManager::GetANSITime( )
{
	SetTime();

	return m_SetTime;
}

unsigned int TimeManager::Time2DWORD( )
{
	SetTime( ) ;

	unsigned int uRet=0 ;

	uRet += GetYear( ) ;
	uRet -= 2000 ;
	uRet =uRet*100 ;

	uRet += GetMonth( )+1 ;
	uRet =uRet*100 ;

	uRet += GetDay( ) ;
	uRet =uRet*100 ;

	uRet += GetHour( ) ;
	uRet =uRet*100 ;

	uRet += GetMinute( ) ;

	return uRet ;
}

void TimeManager::DWORD2Time( unsigned int Date, tm* TM )
{
	assert( TM ) ;
	memset( TM, 0, sizeof(tm) ) ;

	TM->tm_year	= ( Date / 100000000 ) + 2000 - 1900 ;
	TM->tm_mon	= ( Date % 100000000 ) / 1000000 - 1 ;
	TM->tm_mday	= ( Date % 1000000 ) / 10000 ;
	TM->tm_hour	= ( Date % 10000 ) / 100 ;
	TM->tm_min	= Date % 100 ;
}

//取得两个DWORDTime的时间差（单位：分钟）, Ret = Date2-Data1
unsigned int TimeManager::DiffDWORDTime( unsigned int Date1, unsigned int Date2 )
{
	tm S_D1, S_D2 ;
	DWORD2Time( Date1, &S_D1 ) ;
	DWORD2Time( Date2, &S_D2 ) ;
	time_t t1,t2 ;
	t1 = mktime(&S_D1) ;
	t2 = mktime(&S_D2) ;
	unsigned int dif = ( unsigned int )( abs( ( int )( difftime(t2,t1)/60 ) ) ) ;
	return dif ;
}

int	TimeManager::IsDiffDay(time_t uPrevANSITime, time_t uNextANSITime)
{
	tm tPrevTm = *localtime((time_t*)&uPrevANSITime);
	tm tCurTm = *localtime((time_t*)&uNextANSITime);
	return tCurTm.tm_yday - tPrevTm.tm_yday;
}

int TimeManager::DiffDayCount(time_t uPrevANSITime, time_t uNextANSITime)
{
		//tm tPrevTm = *localtime((time_t*)&uPrevANSITime);
		//tm tCurTm = *localtime((time_t*)&uNextANSITime);
		//return tCurTm.tm_yday - tPrevTm.tm_yday;

	int dif = ( int )( difftime(uNextANSITime,uPrevANSITime)/(24*60*60) ); 
	return dif;

	//time_t uDaySeconds = 60 * 60 * 24;
	//time_t uPrevDay = uPrevANSITime / uDaySeconds;
	//time_t uNextDay = uNextANSITime / uDaySeconds;
	//return ((uPrevDay > uNextDay)?((int)(uPrevDay - uNextDay)):((int)(uNextDay - uPrevDay)));

}

//例如：20070403表示2007年4月3日
unsigned int TimeManager::Time2Day( )
{
	unsigned int uRet=0 ;

	uRet += GetYear( ) ;

	uRet =uRet*100 ;
	uRet += GetMonth( )+1 ;

	uRet =uRet*100 ;
	uRet += GetDay( ) ;

	return uRet ;
}

unsigned int TimeManager::DiffTime( unsigned int Date1, unsigned int Date2 )
{
	tm S_D1, S_D2 ;
	ConvertUT( Date1, &S_D1 ) ;
	ConvertUT( Date2, &S_D2 ) ;
	time_t t1,t2 ;
	t1 = mktime(&S_D1) ;
	t2 = mktime(&S_D2) ;
	unsigned int dif = ( unsigned int )( abs( ( int )( difftime(t2,t1) ) ) * 1000 ) ;
	return dif ;
}

void TimeManager::ConvertUT( unsigned int Date, tm* TM )
{
	assert(TM) ;
	memset( TM, 0, sizeof(tm) ) ;
	TM->tm_year = ((Date>>26)&0xf)+100 ;
	TM->tm_mon  = (Date>>22)&0xf ;
	TM->tm_mday = (Date>>17)&0x1f ;
	TM->tm_hour = (Date>>12)&0x1f ;
	TM->tm_min  = (Date>>6) &0x3f ;
	TM->tm_sec  = (Date)    &0x3f ;
}

void TimeManager::ConvertTU( tm* TM, unsigned int& Date )
{
	assert( TM ) ;
	Date = 0 ;
	Date += (TM->tm_year%10) & 0xf ;
	Date = (Date<<4) ;
	Date += TM->tm_mon & 0xf ;
	Date = (Date<<5) ;
	Date += TM->tm_mday & 0x1f ;
	Date = (Date<<5) ;
	Date += TM->tm_hour & 0x1f ;
	Date = (Date<<6) ;
	Date += TM->tm_min & 0x3f ;
	Date = (Date<<6) ;
	Date += TM->tm_sec & 0x3f ;
}

unsigned int TimeManager::GetDayTime( )
{
	time_t st ;
	time( &st ) ;
	tm* ptm = localtime( &m_SetTime ) ;

	unsigned int uRet=0 ;

	uRet  = (ptm->tm_year-100)*1000 ;
	uRet += ptm->tm_yday ;

	return uRet ;
}

unsigned int TimeManager::GetHourTime( )
{
	time_t st ;
	time( &st ) ;
	tm* ptm = localtime( &m_SetTime ) ;

	unsigned int uRet=0 ;
	if( ptm->tm_year + 1900 == 2008 )
	{
		uRet= 365;
	}
	uRet += ptm->tm_yday ;
	uRet *= 100 ;
	uRet += ptm->tm_hour*4 ;
	uRet += (unsigned int)(ptm->tm_min/15) ;

	return uRet ;
}

//取得以周为单位的时间值, 千位数代表年份，其他三位代表时间（周数）
unsigned int TimeManager::GetWeekTime( )
{
	time_t st ;
	time( &st ) ;
	tm* ptm = localtime( &m_SetTime ) ;

	unsigned int uRet=0 ;

	uRet  = (ptm->tm_year-100)*1000 ;

	if(ptm->tm_yday <= ptm->tm_wday)
		return uRet;

	int it = ptm->tm_yday - ptm->tm_wday;
	uRet += static_cast<unsigned int>(ceil((double)(it/7)));

	return uRet ;
}

// 得到tm结构的时间  20100723 BLL
tm  TimeManager::GetFormatTime( )
{
	SetTime();

	return m_TM;
}