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

	//��ǰʱ�����ֵ����ʼֵ����ϵͳ��ͬ������
	//���ص�ֵΪ��΢�λ��ʱ��ֵ
	unsigned int			CurrentTime( ) ;
	//ֱ�ӷ��أ�������ϵͳ�ӿ�
	unsigned int			CurrentSavedTime( ){ return m_CurrentTime ; } ;
	//ȡ�÷������˳�������ʱ��ʱ�����ֵ
	unsigned int			StartTime( ){ return m_StartTime ; } ;

	//����ǰ��ϵͳʱ���ʽ����ʱ���������
	void			SetTime( ) ;

	// �õ���׼ʱ��
	time_t			GetANSITime( );

	// �õ�tm�ṹ��ʱ�� 20100723 BLL
	tm  TimeManager::GetFormatTime( );

public :
	//***ע�⣺
	//���½ӿڵ���û��ϵͳ���ã�ֻ���ʱ��������ڵ�����
	//

	//ȡ������ʱ��ʱ��ġ��ꡢ�¡��ա�Сʱ���֡��롢���ڵ�ֵ��
	int				GetYear( ){ return m_TM.tm_year+1900 ; } ;	//[1900,????]
	int				GetMonth( ){ return m_TM.tm_mon+1 ; } ;		//[1,12]
	int				GetDay( ){ return m_TM.tm_mday ; } ;		//[1,31]
	int				GetHour( ){ return m_TM.tm_hour ; } ;		//[0,23]
	int				GetMinute( ){ return m_TM.tm_min ; } ;		//[0,59]
	int				GetSecond( ){ return m_TM.tm_sec ; } ;		//[0,59]
	//ȡ�õ�ǰ�����ڼ���0��ʾ�������죬1��6��ʾ������һ��������
	unsigned int			GetWeek( ){ return m_TM.tm_wday ; } ;
	//����ǰ��ʱ�䣨�ꡢ�¡��ա�Сʱ���֣�ת����һ��unsigned int����ʾ
	//���磺0,507,211,233 ��ʾ "2005.07.21 12:33"
	unsigned int			Time2DWORD( ) ;
	void			DWORD2Time( unsigned int Date, tm* TM ) ;
	//ȡ������DWORDTime��ʱ����λ�����ӣ�, Ret = Date2-Data1
	unsigned int			DiffDWORDTime( unsigned int Date1, unsigned int Date2 ) ;
	// �Ƚ������Ĳ��
	int				DiffDayCount(time_t uPrevANSITime, time_t uNextANSITime);
	//�Ƚ��ǲ���ͬһ��
	int				IsDiffDay(time_t uPrevANSITime, time_t uNextANSITime);

	//���磺20070403��ʾ2007��4��3��
	unsigned int			Time2Day( ) ;
	//ȡ�õ�ǰ������[4bit 0-9][4bit 0-11][5bit 0-30][5bit 0-23][6bit 0-59][6bit 0-59]
	unsigned int			CurrentDate( ) ;
	//ȡ�÷����������������ʱ�䣨���룩
	unsigned int			RunTime( ){ 
		CurrentTime( ) ;
		return (m_CurrentTime-m_StartTime);  
	} ;
	//ȡ����������ʱ���ʱ����λ�����룩, Ret = Date2-Data1
	unsigned int			DiffTime( unsigned int Date1, unsigned int Date2 ) ;
	//��һ��unsigned int������ת����һ��tm�ṹ
	void			ConvertUT( unsigned int Date, tm* TM ) ;
	//��һ��tm�ṹת����һ��unsigned int������
	void			ConvertTU( tm* TM, unsigned int& Date ) ;
	//ȡ������Ϊ��λ��ʱ��ֵ, ǧλ��������ݣ�������λ����ʱ�䣨������
	unsigned int			GetDayTime( ) ;
	//ȡ����СʱΪ��λ��ʱ��ֵ��
	//"12723��ʾ����ȵ�127���5(23/4)��ĵ�3(23%4)����ʱ��"
	unsigned int			GetHourTime( ) ;

	//ȡ������Ϊ��λ��ʱ��ֵ, ǧλ��������ݣ�������λ����ʱ�䣨������
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
