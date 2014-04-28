#ifndef __PACKET_H__
#define __PACKET_H__

#include <list>
#include "Utility/Type.h"
#include "libmsgpack/msgpack.hpp"
#include "Utility/GLog.h"

class Stub;
class IMsgFactory;

class Msg
{
	friend class Stub;

public:
	virtual MSG_ID_t	ID() = 0;
			MSG_LEN_t	Len() { return m_uLen;	}
			void		Len(MSG_LEN_t len) { m_uLen = len;	}

	virtual IMsgFactory* GetFactory() = 0;

protected:
	MSG_LEN_t	m_uLen;
};

class IMsgListener;
class MsgFactoryManager;

class IMsgFactory
{
public:
	typedef int	FuncAddr;

private:
	friend class MsgFactoryManager;
	friend class Stub;

	struct Handle
	{
		IMsgListener*	m_pListener;
		FuncAddr		m_pFuncHandle;
	};
	typedef std::list< Handle* >	HandleList;

public:
	virtual MSG_ID_t	GetMsgID() = 0;
	virtual Msg*		GetMsg() = 0;

	virtual bool			Read(Msg* pMsg, const char* pBuffer, const size_t uLen) = 0;
	virtual size_t			Write(Msg* pMsg, char* pBuffer, size_t uLen) = 0;

protected:
	virtual bool	RegisterHandle(IMsgListener* pListener, FuncAddr pFuncHandle);
	virtual bool	HandleMsg(Msg* pMsg, Stub* pStub);

protected:
	Msg*		m_pMsg;
private:
	HandleList	m_lstHandles;
};

class IMsgListener
{
	friend class IMsgFactory;
	friend class MsgFactoryManager;
public:
	virtual const char*	SystemName()	= 0;

protected:
	virtual bool	__HandleMsg(Msg* pPacket, Stub* pStub, IMsgFactory::FuncAddr pFunc ) { return true;	}
	virtual void	__InitMsgHandles() {}
};

template< class T >
class MsgFactory : public IMsgFactory
{
	friend class MsgFactoryManager;

public:
	MsgFactory() 
	{
		m_pMsg = (Msg*)(new T);
	}

	virtual ~MsgFactory()
	{
		delete m_pMsg;
	}

	bool Read(Msg* pMsg, const char* pBuffer, const size_t uLen)
	{
		T* pNetMsg = static_cast<T*>(pMsg);
		if( !pNetMsg )
			return false;

		// 读取
		try {
			msgpack::unpacked msg;
			msgpack::unpack(&msg, pBuffer, uLen);
			msgpack::object obj = msg.get();
			obj.convert( &(pNetMsg->body) );
		} catch (...) {
			g_Log.SaveLog(LOG_LV_ERROR, "Read msg error! <%d, %d>", pMsg->ID(), uLen);
			return false;
		}

		return true;
	}

	size_t Write(Msg* pMsg, char* pBuffer, size_t uLen)
	{
		T* pNetMsg = static_cast<T*>(pMsg);
		if( !pNetMsg )
			return false;

		size_t uIDLen = sizeof(MSG_ID_t);
		size_t uLenLen = sizeof(MSG_LEN_t);

		// 打包
		msgpack::sbuffer body;
		try {	
			msgpack::pack(&body, pNetMsg->body);
		} catch (...) {
			g_Log.SaveLog(LOG_LV_ERROR, "Write msg error! <%d> reason: pack body", pMsg->ID());
			return 0;
		}

		MSG_LEN_t len = body.size();
		MSG_LEN_t total_len = uIDLen + uLenLen + len;

		if (total_len > uLen) {
			g_Log.SaveLog(LOG_LV_ERROR, "Write msg error! <%d> reason: buffer overflow <%d>", pMsg->ID(), len);
			return 0;
		}

		pMsg->Len(len);
		MSG_ID_t uMsgID = pMsg->ID();

		// 头部
		memcpy(pBuffer, &uMsgID, uIDLen);
		memcpy(pBuffer + uIDLen, &len, uLenLen);
		memcpy(pBuffer + uIDLen + uLenLen, (char*)body.data(), len);
		
		return total_len;
	}

	inline Msg* GetMsg() 
	{
		return m_pMsg;
	}
};

#endif // __PACKET_H__

