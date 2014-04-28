#ifndef __TEST_SYSTEM2_H__
#define __TEST_SYSTEM2_H__

#include "MsgDefine.h"
#include "Application.h"

class TestSystem2 : public SubSystem
{
public:
	TestSystem2(void);
	~TestSystem2(void);

	const char*	SystemName()	{ return "TestSystem2";	}
	const int	SystemID()		{ return 2;				}		
	
	bool		Initialize( );
	bool		Finalize( );
	void		HeartBeat( int nSkip, float fTimePass);

protected:
	bool		OnConnected(Msg* pMsg, Stub* pStub);
	bool		OnDisconnected(Msg* pMsg, Stub* pStub);
	bool		OnRecvClientMsg(Msg* pMsg, Stub* pStub);

	DECLARE_MSG_HANDLE_BEGIN(TestSystem2)
		MSG_HANDLE(Connected, OnConnected);
		MSG_HANDLE(Disconnected, OnDisconnected);
		MSG_HANDLE(ClientMsg, OnRecvClientMsg);
	DECLARE_MSG_HANDLE_END
};

#endif // __TEST_SYSTEM_H__

