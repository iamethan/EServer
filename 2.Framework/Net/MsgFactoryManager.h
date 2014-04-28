#ifndef __MSG_FACTORY_MANAGER_H__
#define __MSG_FACTORY_MANAGER_H__

#include <map>
#include "Msg.h"

class MsgFactoryManager
{
	// packet struct
	struct MSG_FACTORY 
	{
		IMsgFactory*	m_pFactory;			// factory ptr
		size_t			m_uMsgCount;		// allocated packets count
	};
	typedef std::map< MSG_ID_t, MSG_FACTORY* >	MsgFactoryMap;

public :
	
	MsgFactoryManager();
	~MsgFactoryManager();

public :
	IMsgFactory*	GetFactory(MSG_ID_t uMsgID);

	// 创建消息
	Msg*			GetMsg(MSG_ID_t uMsgID);

	// 处理
	bool			HandleMsg(Msg* pMsg, Stub* pStub);
	bool			RegisteMsgHandle(MSG_ID_t uMsgID, IMsgListener* pListener, IMsgFactory::FuncAddr pHandleFunc );

	// 
	void			AddFactory(IMsgFactory* pFactory);
	bool			FindFactory(MSG_ID_t uMsgID);

	size_t			GetCount() { return m_mapFactories.size();	}

protected:
	MSG_FACTORY*	GetFactoryStruct(MSG_ID_t uMsgID);

private :
	// packet factories
	MsgFactoryMap	m_mapFactories;
};

extern MsgFactoryManager	g_MsgFactoryManager;

//////////////////////////////////////////////////////
// 处理消息
#define DECLARE_MSG_HANDLE_BEGIN( CLASS ) \
	typedef bool (CLASS::*__MSG_HANDLE) ( Msg* pMsg, Stub* pStub );	\
	typedef CLASS	__THIS_CLASS__;  \
	bool __HandleMsg( Msg* pMsg, Stub* pStub, IMsgFactory::FuncAddr pFunc )	\
	{	\
		__MSG_HANDLE pCallBack = *((__MSG_HANDLE*)&pFunc);	\
		return (this->*pCallBack)( pMsg, pStub );		\
	}	\
	void __InitMsgHandles( ) {

#define DECLARE_MSG_HANDLE_END		}

#define MSG_HANDLE( MSG, HANDLEFUNC )	{	\
	if (g_MsgFactoryManager.GetFactory(__##MSG##__FACTORY__::GetID()) == NULL) { \
		g_MsgFactoryManager.AddFactory(new __##MSG##__FACTORY__()); } \
	__MSG_HANDLE func = &__THIS_CLASS__::HANDLEFUNC;	\
	IMsgFactory::FuncAddr pFunc = *((IMsgFactory::FuncAddr*)(&func));	\
	g_MsgFactoryManager.RegisteMsgHandle(__##MSG##__FACTORY__::GetID(), this, pFunc); }


#define DECLEAR_MSG_BEGIN( MSG, PID )	\
	class MSG; \
	class __##MSG##__FACTORY__ : public MsgFactory<MSG> { public: \
	MSG_ID_t	GetMsgID() { return PID;	} \
	static MSG_ID_t	GetID() { return PID;	}	}; \
	class MSG : public Msg { \
	public:	\
	MSG_ID_t ID() { return PID; } \
	IMsgFactory* GetFactory() { IMsgFactory* pFactory = g_MsgFactoryManager.GetFactory(this->ID()); \
	if (pFactory == NULL) { pFactory = (IMsgFactory*)(new __##MSG##__FACTORY__()); \
	g_MsgFactoryManager.AddFactory(pFactory); } return pFactory; }

#define DECLEAR_MSG_END	};

// 预定义消息
#define MSGID_CONNECTED		1
#define	MSGID_CONNECTFAILED	2
#define	MSGID_DISCONNECTED	3

DECLEAR_MSG_BEGIN(Connected, MSGID_CONNECTED)
	struct PacketBody
	{
		MSGPACK_DEFINE();
	};

	PacketBody body;    // 包体
DECLEAR_MSG_END;

DECLEAR_MSG_BEGIN(ConnectFailed, MSGID_CONNECTFAILED)
	struct PacketBody
	{
		int	m_nCode;

		MSGPACK_DEFINE(m_nCode);
	};

	PacketBody body;    // 包体
DECLEAR_MSG_END;

DECLEAR_MSG_BEGIN(Disconnected, MSGID_DISCONNECTED)
	struct PacketBody
	{
		int m_nCode;

		MSGPACK_DEFINE(m_nCode);
	};

	PacketBody body;    // 包体
DECLEAR_MSG_END;

#endif // __MSG_FACTORY_MANAGER_H__

