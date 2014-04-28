#include "StubManager.h"
#include "Stub.h"
#include "Utility/GLog.h"

StubManager g_StubManager;

StubManager::StubManager(void)
	: m_StubIdGen(1, 1000000, -1)
{
	
}

StubManager::~StubManager(void)
{
	
}

bool StubManager::Initialize()
{
	return true;
}

bool StubManager::Finialize()
{
	STUB_MAP::iterator iterStubEnd = m_mapStubs.end();
	for (STUB_MAP::iterator iter = m_mapStubs.begin(); iter != iterStubEnd; ++iter) {
		delete iter->second;
	}
	m_mapStubs.clear();

	return true;
}

void StubManager::HeartBeat(int nSkip, float fTimePass)
{
	uv_run(uv_default_loop(), UV_RUN_NOWAIT);
}

bool StubManager::ListenPort(int nPort)
{
	Stub* pStub = NewStub();
	return pStub->Listen(nPort);
}

Stub* StubManager::NewStub()
{
	int id = m_StubIdGen.GetUnusedID();
	if (id < 0) {
		g_Log.SaveLog(LOG_LV_ERROR, "stub id has full...", id);
		return NULL;
	}

	Stub* pStub = new Stub(id);
	if (pStub) {
		m_StubIdGen.UseID(id);

		m_mapStubs.insert(STUB_MAP::value_type(id, pStub));
	}
	
	return pStub;
}

void StubManager::DeleteStub(Stub* pStub)
{
	int id = pStub->m_nStubID;
	STUB_MAP::iterator iter = m_mapStubs.find(id);

	if (iter == m_mapStubs.end()) {
		g_Log.SaveLog(LOG_LV_ERROR, "delete stub failed! can not find the stub<%d>", id);
		return;
	}

	m_StubIdGen.RecycleID(id);
	m_mapStubs.erase(iter);
	delete pStub;
}
