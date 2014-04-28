#include "Net.h"
#include "TestSystem.h"

TestSystem::TestSystem(void)
{
}


TestSystem::~TestSystem(void)
{
}

bool TestSystem::Initialize( )
{
	return true;
}

bool TestSystem::Finalize( )
{
	return true;
}

void TestSystem::HeartBeat( int nSkip, float fTimePass)
{
}

bool TestSystem::OnConnected(Msg* pMsg, Stub* pStub)
{
	g_Log.SaveLog(LOG_LV_NORMAL, "==========on connected %s, %d", pStub->m_sIP.c_str(), pStub->m_nPort);

	return true;
}

bool TestSystem::OnDisconnected(Msg* pMsg, Stub* pStub)
{
	g_Log.SaveLog(LOG_LV_NORMAL, "==========on disconnected %s, %d", pStub->m_sIP.c_str(), pStub->m_nPort);
	return true;
}

bool TestSystem::OnRecvServerMsg(Msg* pMsg, Stub* pStub)
{
	ServerMsg* pNetMsg = (ServerMsg*)pMsg;

	g_Log.SaveLog(LOG_LV_NORMAL, "============recv: %d %d %s", pNetMsg->body.m_a, pNetMsg->body.m_b, pNetMsg->body.m_s.c_str());

	ClientMsg msg;
	msg.body.m_a = 10;
	msg.body.m_v.push_back(0);
	msg.body.m_v.push_back(1);
	msg.body.m_v.push_back(10);
	msg.body.m_v.push_back(20);
	pStub->Send(&msg);

	return true;
}
