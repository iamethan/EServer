#ifndef __G_LOG_H__
#define __G_LOG_H__

#include "File.h"
#include "TimeManager.h"

enum LOG_LV
{
	LOG_LV_NORMAL = 0 ,
	LOG_LV_WARNING ,
	LOG_LV_ERROR ,
	LOG_LV_DUMP ,

	LOG_LV_NUMBER ,
};

#define DEFAULT_LOG_CACHE_SIZE 1024*1024*4
#define LOG_BUFF_TEMP 4096
#define LOG_NAME_TEMP 128
#define LOG_FILE_NAME_MAX_SIZE	128

class GLog
{
public :
	GLog( const char* fileName = "default" ) ;
	~GLog( ) ;

	bool			Init( int cachesize=DEFAULT_LOG_CACHE_SIZE ) ;

	void			ChangeFileName( const char* fileName );

	//向日志缓存写入日志信息
	void			SaveLog( LOG_LV lv, char* msg, ... ) ;

	//将日志内存数据写入文件
	void			FlushLog( LOG_LV lv ) ;

	//取得日志有效数据大小
	int				GetLogSize( LOG_LV lv ){ return m_LogPos[lv] ; }

	//取得保存日志的文件名称
	void			GetLogName( LOG_LV lv, char* szName ) ;

	//刷新每个日志文件
	void			FlushLog_All( ) ;

	//取得日期天数
	unsigned int			GetDayTime( ){ return m_DayTime ; }
	//设置日期天数
	void			SetDayTime( unsigned int daytime ){ m_DayTime = daytime ; }

private :
	char*			m_LogCache[LOG_LV_NUMBER] ;	//日志内存区
	int				m_LogPos[LOG_LV_NUMBER] ;		//日志当前有效数据位置
	int				m_CacheSize ;
	unsigned int	m_DayTime ;

	char			m_LogName[LOG_FILE_NAME_MAX_SIZE];

	TimeManager		m_Time;
};

extern GLog g_Log;

#if defined __LINUX__
	#define SaveCodeLog()	(g_Log.SaveLog( LOG_LV_ERROR, "%s %d %s", __FILE__,__LINE__,__PRETTY_FUNCTION__ ))
#else
	#define SaveCodeLog()	(g_Log.SaveLog( LOG_LV_ERROR, "%s %d %s", __FILE__,__LINE__,__FUNCTION__ ))
#endif

#endif // __G_LOG_H__
