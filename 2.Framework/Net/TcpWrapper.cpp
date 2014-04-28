#include "TcpWrapper.h"
#include "Stub.h"
#include "StubManager.h"
#include <assert.h>
#include "Utility/GLog.h"

bool TcpWrapper::Listen(uv_tcp_t* pUvTcp, int nPort)
{
	uv_tcp_nodelay(pUvTcp, false);
	uv_tcp_keepalive(pUvTcp, true, 20*60);

	uv_tcp_bind(pUvTcp, uv_ip4_addr("0.0.0.0", nPort));

	int r = uv_listen((uv_stream_t*)pUvTcp, 128, __OnNewConnection);
	if (r) {
		g_Log.SaveLog(LOG_LV_ERROR, "Listen error %s", uv_err_name(r));
		return false;
	}

	return true;
}

bool TcpWrapper::Connect(uv_tcp_t* pUvTcp, const char* sIP, int nPort)
{
	uv_connect_t* req = new uv_connect_t;
	int r = uv_tcp_connect(req, pUvTcp, uv_ip4_addr(sIP, nPort), __OnConnected);
	if (r) {
		delete req;
		return false;
	}

	return true;
}

bool TcpWrapper::Close(uv_tcp_t* pUvTcp)
{
	uv_close((uv_handle_t*)pUvTcp, __AfterClosed);
	return true;
}

bool TcpWrapper::Send(uv_tcp_t* pUvTcp, char* buff, size_t len)
{
	uv_write_t* req = new uv_write_t;
	uv_buf_t req_buf = uv_buf_init(buff, len);

	int r = uv_write(req, (uv_stream_t*)pUvTcp, &req_buf, 1, __AfterWrite);
	if (r) {
		g_Log.SaveLog(LOG_LV_ERROR, "write socket failed! reason:<%s>", uv_err_name(r));
		return false;
	}

	return true;
}

const char* TcpWrapper::GetErrorDesc(int err)
{
	return uv_err_name(err);
}

void TcpWrapper::GetPeerAddr(uv_tcp_t* pUvTcp, std::string& sIP, int& nPort)
{
	sockaddr_in addr;
	int len = sizeof(addr);

	uv_tcp_getpeername(pUvTcp, (sockaddr*)&addr, &len);

	sIP = inet_ntoa(addr.sin_addr);
	nPort = ntohs(addr.sin_port);
}

void TcpWrapper::__OnNewConnection(uv_stream_t* server, int status)
{
	if (status != 0) {
		g_Log.SaveLog(LOG_LV_ERROR, "Connect error %s", uv_err_name(status));
		return;
	}

	Stub* pStub = g_StubManager.NewStub();
	if (pStub == NULL) {
		g_Log.SaveLog(LOG_LV_ERROR, "create new stub failed!");
		return;
	}

	int r = uv_accept(server, (uv_stream_t*)&pStub->m_uvTcp);
	if (r) {
		g_Log.SaveLog(LOG_LV_ERROR, "Connect accept error %s", uv_err_name(r));
		delete pStub;
		return;
	}

	pStub->OnConnected();

	uv_read_start((uv_stream_t*)&pStub->m_uvTcp, __OnAlloc, __OnRead);
}

void TcpWrapper::__OnConnected(uv_connect_t* req, int status)
{
	Stub* pStub = (Stub*)req->handle->data;

	assert((void*)&pStub->m_uvTcp == (void*)req->handle);

	if (status == 0) {
		pStub->OnConnectSuccessed();
	} else {
		pStub->OnConnectFailed(status);
	}

	uv_read_start((uv_stream_t*)&pStub->m_uvTcp, __OnAlloc, __OnRead);

	delete req;
}

uv_buf_t TcpWrapper::__OnAlloc(uv_handle_t* handle, size_t suggested_size)
{
	Stub* pStub = (Stub*)handle->data;
	
	assert((void*)&pStub->m_uvTcp == (void*)handle);

	return pStub->OnAlloc(suggested_size);
}

void TcpWrapper::__OnRead(uv_stream_t* stream, ssize_t nread, uv_buf_t buff)
{
	Stub* pStub = (Stub*)stream->data;

	assert((void*)&pStub->m_uvTcp == (void*)stream);

	if (nread < 0) {
		uv_shutdown_t* req = new uv_shutdown_t;
		uv_shutdown(req, stream, __AfterShutdown);
	} else {
		pStub->OnRead(nread, buff);
	}
}

void TcpWrapper::__AfterWrite(uv_write_t* req, int status)
{
	if (status != 0) {
		uv_close((uv_handle_t*)req->handle, __AfterClosed);
	}

	delete req;
}

void TcpWrapper::__AfterShutdown(uv_shutdown_t* req, int status)
{
	uv_close((uv_handle_t*)req->handle, __AfterClosed);
	delete req;
}

void TcpWrapper::__AfterClosed(uv_handle_t* handle)
{
	Stub* pStub = (Stub*)handle->data;

	assert((void*)&pStub->m_uvTcp == (void*)handle);

	pStub->OnDisconnected();

	g_StubManager.DeleteStub(pStub);
}
