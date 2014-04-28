#include "TestSystem2.h"

TestSystem2::TestSystem2(void)
{
}


TestSystem2::~TestSystem2(void)
{
}

bool TestSystem2::Initialize( )
{
	return true;
}

bool TestSystem2::Finalize( )
{
	return true;
}

void TestSystem2::HeartBeat( int nSkip, float fTimePass)
{
}

bool TestSystem2::OnConnected(Msg* pMsg, Stub* pStub)
{
	g_Log.SaveLog(LOG_LV_NORMAL, "-----------on connected %s, %d", pStub->m_sIP.c_str(), pStub->m_nPort);


	return true;
}

bool TestSystem2::OnDisconnected(Msg* pMsg, Stub* pStub)
{
	g_Log.SaveLog(LOG_LV_NORMAL, "------------on disconnected %s, %d", pStub->m_sIP.c_str(), pStub->m_nPort);
	return true;
}

bool TestSystem2::OnRecvClientMsg(Msg* pMsg, Stub* pStub)
{
	ClientMsg* pNetMsg = (ClientMsg*)pMsg;

	g_Log.SaveLog(LOG_LV_NORMAL, "----------recv client msg %d %d", pNetMsg->body.m_a, pNetMsg->body.m_v.size());


	return true;
}
