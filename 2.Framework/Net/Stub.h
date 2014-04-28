#ifndef __STUB_H__
#define __STUB_H__

#include <string>
#include "libuv/include/uv.h"

enum STUB_TYPE 
{
	STUB_TYPE_INVALID	= -1,	// 无效
	STUB_TYPE_SERVER	= 0,	// 服务端
	STUB_TYPE_CLIENT	= 1,	// 客户端
};

#define	STUB_INPUTBUFFER_SIZE	8192
#define STUB_OUTPUTBUFFER_SIZE	4096

class TcpWrapper;
class Msg;
class StubManager;

class Stub
{
	friend class StubManager;
	friend class TcpWrapper;

public:
	Stub(int nID);
	virtual ~Stub();

	int			StubID();
	STUB_TYPE	StubType();

public:
	// server
	bool	Listen(int nPort);

	// client
	bool	Connect(const char* sIp, int nPort);

	// common
	bool	Send(Msg* pMsg);
	bool	Close();

public:
	void		OnConnectSuccessed();
	void		OnConnectFailed(int status);
	void		OnConnected();
	void		OnDisconnected();
	uv_buf_t	OnAlloc(size_t suggested_size);
	void		OnRead(ssize_t nread, uv_buf_t buff);

public:
	std::string	m_sIP;
	int			m_nPort;

protected:
	int			m_nStubID;
	STUB_TYPE	m_eStubType;

	uv_tcp_t	m_uvTcp;

	char*		m_pInputBuffers;
	size_t		m_uInputBufferPos;
	char*		m_pOutputBuffers;
};

#endif // __STUB_H__

