#ifndef __APP_H__
#define __APP_H__

#include <map>
#include "Utility/GTimer.h"

#define APP_RUN_DEFAULT_LOOP_FPS     30.0f

class SubSystem;

class App
{
	typedef std::map<int, SubSystem*>	SubSystemPtrMap;

public:
	App(void);
	~App(void);

	virtual bool	Initialize();
	virtual bool	Finalize();

	virtual void	InitSystems( VOID ) = 0;

	bool			Quit();
	void			Quit(bool bQuit);

	virtual void	Run(float fRPS=APP_RUN_DEFAULT_LOOP_FPS);
	virtual void	HeartBeat(int iSkip, float fTimePass) {}

	// ×ÓÏµÍ³
	bool			RegisterSystem(SubSystem* pSys);

protected:
	void			__HeartBeat(int nSkip, float fTimePass);

protected:
	float			m_fMaxRPS; 
	float			m_fIntervalPerRun; 
	float			m_fRPS;                
	float			m_fAverageTimeUsedPerSec; 
	int				m_nRuns;             
	int				m_nTotalFrame;

	bool			m_bDumpRPSInfo;
	bool			m_bQuit;

	GTimer			m_Timer;

private:
	SubSystemPtrMap	m_mapSystems;
};

#endif // __APP_H__

