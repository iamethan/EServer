#include "App.h"
#include "Utility/GameUtil.h"
#include "Net/StubManager.h"
#include "SubSystem.h"
#include "Utility/GLog.h"

App::App(void)
	: m_bQuit(false)
{
}

App::~App(void)
{
}

bool App::Initialize()
{
	InitSystems();
	g_Log.SaveLog( LOG_LV_NORMAL, "Initialized Systems, total num<%d>",
		m_mapSystems.size() );

	if (!g_StubManager.Initialize()) {
		return false;
	}

	SubSystemPtrMap::iterator itEnd = m_mapSystems.end();
	for( SubSystemPtrMap::iterator it = m_mapSystems.begin(); it != itEnd; ++it )
	{
		g_Log.SaveLog( LOG_LV_NORMAL, "Initialize System<%s> ....", it->second->SystemName() );
		if( !it->second->__Initialize() )
		{
			g_Log.SaveLog( LOG_LV_ERROR, "Initialize System<%s> failed!", it->second->SystemName() );
			continue;
		}
		
		g_Log.SaveLog( LOG_LV_NORMAL, "Initialize System<%s> successed!", it->second->SystemName() );
	}

	return true;
}

bool App::Finalize()
{
	SubSystemPtrMap::iterator itEnd = m_mapSystems.end();
	for( SubSystemPtrMap::iterator it = m_mapSystems.begin(); it != itEnd; ++it )
	{
		g_Log.SaveLog( LOG_LV_NORMAL, "Finalize System<%s> ....", it->second->SystemName() );
		if( !it->second->__Finalize() )
		{
			g_Log.SaveLog( LOG_LV_ERROR, "Finalize System<%s> failed!", it->second->SystemName() );
			continue;
		}
		
		g_Log.SaveLog( LOG_LV_NORMAL, "Finalize System<%s> successed!", it->second->SystemName() );
	}

	if (!g_StubManager.Finialize()) {
		return false;	
	}

	return true;
}

bool App::Quit()
{
	return true;
}

void App::Quit(bool bQuit)
{
}

bool App::RegisterSystem(SubSystem* pSys)
{
	SubSystemPtrMap::iterator it = m_mapSystems.find( pSys->SystemID() );
	if( it != m_mapSystems.end() )
	{
		g_Log.SaveLog( LOG_LV_ERROR, "App::RegisterSystem<%d, %s> Error, duplicated id",
			pSys->SystemID(), pSys->SystemName() );
		return false;
	}

	if( !m_mapSystems.insert( SubSystemPtrMap::value_type( pSys->SystemID(), pSys ) ).second )
	{
		g_Log.SaveLog( LOG_LV_ERROR, "App::RegisterSystem<%d, %s> Error when insert into map",
			pSys->SystemID(), pSys->SystemName() );
		return false;
	}

	g_Log.SaveLog( LOG_LV_NORMAL, "Register System<%s> Successed!", pSys->SystemName() );

	return true;
}

void App::Run(float fRPS/*=APP_RUN_DEFAULT_LOOP_FPS*/)
{
	m_fMaxRPS = fRPS;

	if( m_fMaxRPS <= 0.0f ) m_fIntervalPerRun = 0.0f;
	else m_fIntervalPerRun = 1.0f / m_fMaxRPS;

	// Do block run and control runs per second
	float	fTimeStamp      = m_fIntervalPerRun;
	float	fTimeSlept      = 0.0f;
	float	fTimeToSleepS   = 0.0f;
	int		nRuns           = 0;
	int		nSkip           = 1;
	// Estimate process usage data
	float	fTimeSleep      = 0.0f;
	// log burst frame
	float	fMaxExceedTime  = 999.0f;
	int		nBurstFrame     = 0;

	// tick timer
	GTimer	tickTimer;
	tickTimer.Reset();

	while (!m_bQuit) 
	{
		// Calculate how long time can sleep
		fTimeToSleepS = fTimeStamp - m_Timer.Seconds();

		// log maximun exceed time
		if (fTimeToSleepS < fMaxExceedTime) 
		{
			fMaxExceedTime = fTimeToSleepS;
			nBurstFrame    = nRuns;
		}

		// Do Sleep & Count sleep rate 
		nSkip = 1;
		if (fTimeToSleepS > 0.001f) 
		{
			MySleep((unsigned int)(fTimeToSleepS * 1000) );
			fTimeSleep  += fTimeToSleepS;
		}
		else 
		{
			nSkip += (int)( -fTimeToSleepS / m_fIntervalPerRun );
		}

		// Calculate next time stamp
		fTimeStamp  += m_fIntervalPerRun * nSkip;

		// heart beat
		__HeartBeat(nSkip, tickTimer.Seconds());
		tickTimer.Reset( );
	}
}

void App::__HeartBeat(int nSkip, float fTimePass)
{
	g_StubManager.HeartBeat(nSkip, fTimePass);

	HeartBeat(nSkip, fTimePass);

	SubSystemPtrMap::iterator itEnd = m_mapSystems.end();
	for( SubSystemPtrMap::iterator it = m_mapSystems.begin(); it != m_mapSystems.end(); ++it )
	{
		it->second->HeartBeat( nSkip, fTimePass );
	}
}
