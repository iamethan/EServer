#ifndef __TCP_WRAPPER_H__
#define __TCP_WRAPPER_H__

#include "libuv/include/uv.h"
#include <string>

class TcpWrapper
{
public:
	static bool Listen(uv_tcp_t* pUvTcp, int nPort);
	static bool Connect(uv_tcp_t* pUvTcp, const char* sIP, int nPort);
	static bool Close(uv_tcp_t* pUvTcp);
	static bool Send(uv_tcp_t* pUvTcp, char* buff, size_t len);

	static const char*	GetErrorDesc(int err);
	static void			GetPeerAddr(uv_tcp_t* pUvTcp, std::string& sIP, int& nPort);

protected:
	static void		__OnNewConnection(uv_stream_t* server, int status);
	static void		__OnConnected(uv_connect_t* req, int status);
	static uv_buf_t __OnAlloc(uv_handle_t* handle, size_t suggested_size);
	static void		__OnRead(uv_stream_t* stream, ssize_t nread, uv_buf_t buff);
	static void		__AfterWrite(uv_write_t* req, int status);
	static void		__AfterShutdown(uv_shutdown_t* req, int status);
	static void		__AfterClosed(uv_handle_t* handle);
};

#endif // __TCP_WRAPPER_H__

