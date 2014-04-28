#ifndef __TIMEMANAGER_H__
#define __TIMEMANAGER_H__

#include <time.h>

#ifdef __LINUX__
#include <sys/utsname.h>
#include <sys/time.h>
#endif

class TimeManager
{
public :
	TimeManager( ) ;
	~TimeManager( ) ;

	bool			Init( ) ;

	//当前时间计数值，起始值根据系统不同有区别
	//返回的值为：微妙单位的时间值
	unsigned int			CurrentTime( ) ;
	//直接返回，不调用系统接口
	unsigned int			CurrentSavedTime( ){ return m_CurrentTime ; } ;
	//取得服务器端程序启动时的时间计数值
	unsigned int			StartTime( ){ return m_StartTime ; } ;

	//将当前的系统时间格式化到时间管理器里
	void			SetTime( ) ;

	// 得到标准时间
	time_t			GetANSITime( );

	// 得到tm结构的时间 20100723 BLL
	tm  TimeManager::GetFormatTime( );

public :
	//***注意：
	//以下接口调用没有系统调用，只针对时间管理器内的数据
	//

	//取得设置时间时候的“年、月、日、小时、分、秒、星期的值”
	int				GetYear( ){ return m_TM.tm_year+1900 ; } ;	//[1900,????]
	int				GetMonth( ){ return m_TM.tm_mon+1 ; } ;		//[1,12]
	int				GetDay( ){ return m_TM.tm_mday ; } ;		//[1,31]
	int				GetHour( ){ return m_TM.tm_hour ; } ;		//[0,23]
	int				GetMinute( ){ return m_TM.tm_min ; } ;		//[0,59]
	int				GetSecond( ){ return m_TM.tm_sec ; } ;		//[0,59]
	//取得当前是星期几；0表示：星期天，1～6表示：星期一～星期六
	unsigned int			GetWeek( ){ return m_TM.tm_wday ; } ;
	//将当前的时间（年、月、日、小时、分）转换成一个unsigned int来表示
	//例如：0,507,211,233 表示 "2005.07.21 12:33"
	unsigned int			Time2DWORD( ) ;
	void			DWORD2Time( unsigned int Date, tm* TM ) ;
	//取得两个DWORDTime的时间差（单位：分钟）, Ret = Date2-Data1
	unsigned int			DiffDWORDTime( unsigned int Date1, unsigned int Date2 ) ;
	// 比较天数的差别
	int				DiffDayCount(time_t uPrevANSITime, time_t uNextANSITime);
	//比较是不是同一天
	int				IsDiffDay(time_t uPrevANSITime, time_t uNextANSITime);

	//例如：20070403表示2007年4月3日
	unsigned int			Time2Day( ) ;
	//取得当前的日期[4bit 0-9][4bit 0-11][5bit 0-30][5bit 0-23][6bit 0-59][6bit 0-59]
	unsigned int			CurrentDate( ) ;
	//取得服务器启动后的运行时间（毫秒）
	unsigned int			RunTime( ){ 
		CurrentTime( ) ;
		return (m_CurrentTime-m_StartTime);  
	} ;
	//取得两个日期时间的时间差（单位：毫秒）, Ret = Date2-Data1
	unsigned int			DiffTime( unsigned int Date1, unsigned int Date2 ) ;
	//将一个unsigned int的日期转换成一个tm结构
	void			ConvertUT( unsigned int Date, tm* TM ) ;
	//将一个tm结构转换成一个unsigned int的日期
	void			ConvertTU( tm* TM, unsigned int& Date ) ;
	//取得以天为单位的时间值, 千位数代表年份，其他三位代表时间（天数）
	unsigned int			GetDayTime( ) ;
	//取得以小时为单位的时间值，
	//"12723表示本年度第127天的5(23/4)点的第3(23%4)刻钟时间"
	unsigned int			GetHourTime( ) ;

	//取得以周为单位的时间值, 千位数代表年份，其他三位代表时间（周数）
	unsigned int			GetWeekTime( ) ;


public :
	unsigned int			m_StartTime ;
	unsigned int			m_CurrentTime ;
	time_t			m_SetTime ;
	tm				m_TM ;
#ifdef __LINUX__
	struct timeval _tstart, _tend;
	struct timezone tz;
#endif

};

extern TimeManager g_TimeManager;

#endif // __TIMEMANAGER_H__
