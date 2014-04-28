#include "Net.h"
#include "Utility.h"
#include "Application.h"
#include "Client.h"
#include "TestSystem.h"

Client::Client(void)
{
}


Client::~Client(void)
{
}

bool Client::Initialize()
{
	if (!App::Initialize()) {
		return false;
	}

	for (int i = 0; i < 1; ++i) {
		Stub* pClient = g_StubManager.NewStub();
		pClient->Connect("127.0.0.1", 1000);
	}

	return true;
}

bool Client::Finialize()
{
	if (!App::Finalize()) {
		return false;	
	}
	return true;
}

void Client::InitSystems()
{
	m_pTestSystem = new TestSystem();
	RegisterSystem((SubSystem*)m_pTestSystem);
}

int main()
{
	g_Log.ChangeFileName( "./logs/Client" );

	g_Log.SaveLog( LOG_LV_NORMAL, "Client started..." );

	Client client;

	client.Initialize();

	client.Run(60);

	return 0;
}
