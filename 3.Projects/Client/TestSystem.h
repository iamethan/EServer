#ifndef __TEST_SYSTEM_H__
#define __TEST_SYSTEM_H__

#include "Application.h"
#include "MsgDefine.h"

class TestSystem : public SubSystem
{
public:
	TestSystem(void);
	~TestSystem(void);

	const char*	SystemName()	{ return "TestSystem";	}
	const int	SystemID()		{ return 1;				}		
	
	bool		Initialize( );
	bool		Finalize( );
	void		HeartBeat( int nSkip, float fTimePass);

protected:
	bool		OnConnected(Msg* pMsg, Stub* pStub);
	bool		OnDisconnected(Msg* pMsg, Stub* pStub);
	bool		OnRecvServerMsg(Msg* pMsg, Stub* pStub);

	DECLARE_MSG_HANDLE_BEGIN(TestSystem)
		MSG_HANDLE(Connected, OnConnected);
		MSG_HANDLE(Disconnected, OnDisconnected);
		MSG_HANDLE(ServerMsg, OnRecvServerMsg);
	DECLARE_MSG_HANDLE_END
};

#endif // __TEST_SYSTEM_H__

