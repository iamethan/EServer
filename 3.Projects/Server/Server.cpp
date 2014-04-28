#include "Net.h"
#include "Utility.h"
#include "Application.h"
#include "Server.h"
#include "TestSystem.h"
#include "TestSystem2.h"

Server::Server(void)
{
}


Server::~Server(void)
{
}

bool Server::Initialize()
{
	if (!App::Initialize()) {
		return false;
	}

	g_StubManager.ListenPort(1000);

	return true;
}

bool Server::Finialize()
{
	if (!App::Finalize()) {
		return false;	
	}
	return true;
}

void Server::InitSystems()
{
	m_pTestSystem = new TestSystem();
	RegisterSystem((SubSystem*)m_pTestSystem);

	m_pTestSystem2 = new TestSystem2();
	RegisterSystem((SubSystem*)m_pTestSystem2);
}

int main()
{
	g_Log.ChangeFileName( "./logs/Server" );

	g_Log.SaveLog( LOG_LV_NORMAL, "Server started..." );

	Server server;

	server.Initialize();

	server.Run(30);

	return 0;
}
