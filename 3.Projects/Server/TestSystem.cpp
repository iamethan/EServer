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

	ServerMsg msg;
	msg.body.m_a = 10;
	msg.body.m_b = 10;
	msg.body.m_s = "asdfasdf";
	pStub->Send(&msg);

	return true;
}

bool TestSystem::OnDisconnected(Msg* pMsg, Stub* pStub)
{
	g_Log.SaveLog(LOG_LV_NORMAL, "==========on disconnected %s, %d", pStub->m_sIP.c_str(), pStub->m_nPort);
	return true;
}

bool TestSystem::OnRecvClientMsg(Msg* pMsg, Stub* pStub)
{
	ClientMsg* pNetMsg = (ClientMsg*)pMsg;

	g_Log.SaveLog(LOG_LV_NORMAL, "========= recv client msg %d %d", pNetMsg->body.m_a, pNetMsg->body.m_v.size());

	ServerMsg msg;
	msg.body.m_a = 10;
	msg.body.m_b = 100000;
	msg.body.m_s = "asdfasdf";
	pStub->Send(&msg);

	return true;
}
