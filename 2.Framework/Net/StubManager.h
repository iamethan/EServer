#ifndef __STUB_MANAGER_H__
#define __STUB_MANAGER_H__

#include <map>
#include "Utility/IDGenerator.h"

class Stub;

class StubManager
{
public:
	typedef std::map<int, Stub*> STUB_MAP;

public:
	StubManager(void);
	~StubManager(void);

	bool	Initialize();
	bool	Finialize();
	void	HeartBeat(int nSkip, float fTimePass);

	bool	ListenPort(int nPort);

public:
	Stub*	NewStub();
	void	DeleteStub(Stub* pStub);

protected:
	STUB_MAP			m_mapStubs;
	IDGenerator<int>	m_StubIdGen;

};

extern StubManager g_StubManager;

#endif // __STUB_MANAGER_H__

