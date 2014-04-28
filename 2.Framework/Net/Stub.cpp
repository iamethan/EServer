#include "Stub.h"
#include "TcpWrapper.h"
#include "Msg.h"
#include "Utility/GLog.h"
#include "MsgFactoryManager.h"
#include <assert.h>
#include "Utility/GTimer.h"

Stub::Stub(int nID)
{
	uv_tcp_init(uv_default_loop(), &m_uvTcp);
	m_uvTcp.data = this;

	m_nStubID = nID;
	m_eStubType = STUB_TYPE_INVALID;
	m_sIP = "0.0.0.0";
	m_nPort = 0;

	m_pOutputBuffers = new char[STUB_OUTPUTBUFFER_SIZE];
	m_pInputBuffers = new char[STUB_INPUTBUFFER_SIZE];
	m_uInputBufferPos = 0;
}

Stub::~Stub()
{
	delete[] m_pInputBuffers;
	delete[] m_pOutputBuffers;
}

int Stub::StubID()
{
	return m_nStubID;
}

STUB_TYPE Stub::StubType()
{
	return m_eStubType;
}

bool Stub::Listen(int nPort)
{
	g_Log.SaveLog(LOG_LV_NORMAL, "start listen port<%d>...", nPort);

	if (m_eStubType != STUB_TYPE_INVALID) {
		g_Log.SaveLog(LOG_LV_ERROR, "the stub has been used! as<%d, ip:%s, port:%d>", (int)m_eStubType, m_sIP.c_str(), m_nPort);
		return false;
	}

	if (!TcpWrapper::Listen(&m_uvTcp, nPort)) {
		g_Log.SaveLog(LOG_LV_ERROR, "listen port<%d> faliled", nPort);
		return false;
	}

	m_eStubType = STUB_TYPE_SERVER;
	m_nPort = nPort;

	return true;
}

bool Stub::Connect(const char* sIp, int nPort)
{
	g_Log.SaveLog(LOG_LV_NORMAL, "connect to<%s, %d>...", sIp, nPort);

	if (m_eStubType != STUB_TYPE_INVALID) {
		g_Log.SaveLog(LOG_LV_ERROR, "the stub has been used! as<%d, ip:%s, port:%d>", (int)m_eStubType, m_sIP.c_str(), m_nPort);
		return false;
	}

	if (!TcpWrapper::Connect(&m_uvTcp, sIp, nPort)) {
		g_Log.SaveLog(LOG_LV_ERROR, "connect to<%s, %d> failed!...", sIp, nPort);
		return false;
	}

	m_eStubType = STUB_TYPE_CLIENT;
	m_sIP = sIp;
	m_nPort = nPort;

	return true;
}

bool Stub::Send(Msg* pMsg)
{
	IMsgFactory* pFactory = pMsg->GetFactory();
	assert(pFactory != NULL);

	size_t uLen = pFactory->Write(pMsg, m_pOutputBuffers, STUB_OUTPUTBUFFER_SIZE);
	if (uLen == 0) {
		g_Log.SaveLog(LOG_LV_ERROR, "Stub Send Error! write buffer failed<id:%d>", pMsg->ID());
		return false;
	}

	return TcpWrapper::Send(&m_uvTcp, m_pOutputBuffers, uLen);
}

bool Stub::Close()
{
	g_Log.SaveLog(LOG_LV_NORMAL, "close the socket<%s, %d>", m_sIP.c_str(), m_nPort);

	return TcpWrapper::Close(&m_uvTcp);
}

void Stub::OnConnectSuccessed()
{
	g_Log.SaveLog(LOG_LV_NORMAL, "on connect<%s, %d> successed", m_sIP.c_str(), m_nPort);

	Msg* pMsg = g_MsgFactoryManager.GetMsg(MSGID_CONNECTED);
	g_MsgFactoryManager.HandleMsg(pMsg, this);
}

void Stub::OnConnectFailed(int status)
{
	g_Log.SaveLog(LOG_LV_NORMAL, "on connect<%s, %d> failed, reason:%s", m_sIP.c_str(), m_nPort, TcpWrapper::GetErrorDesc(status));

	ConnectFailed* pMsg = (ConnectFailed*)g_MsgFactoryManager.GetMsg(MSGID_CONNECTFAILED);
	pMsg->body.m_nCode = status;
	g_MsgFactoryManager.HandleMsg(pMsg, this);
}

void Stub::OnConnected()
{
	TcpWrapper::GetPeerAddr(&m_uvTcp, m_sIP, m_nPort);

	g_Log.SaveLog(LOG_LV_NORMAL, "on connected<%s, %d>", m_sIP.c_str(), m_nPort);

	Msg* pMsg = g_MsgFactoryManager.GetMsg(MSGID_CONNECTED);
	g_MsgFactoryManager.HandleMsg(pMsg, this);
}

void Stub::OnDisconnected()
{
	g_Log.SaveLog(LOG_LV_NORMAL, "on disconnected<%s, %d>", m_sIP.c_str(), m_nPort);

	Msg* pMsg = g_MsgFactoryManager.GetMsg(MSGID_DISCONNECTED);
	g_MsgFactoryManager.HandleMsg(pMsg, this);
}

uv_buf_t Stub::OnAlloc(size_t suggested_size)
{
	return uv_buf_init(m_pInputBuffers + m_uInputBufferPos, STUB_INPUTBUFFER_SIZE - m_uInputBufferPos);
}

void Stub::OnRead(ssize_t nread, uv_buf_t buff)
{
	GTimer timer;

	m_uInputBufferPos += nread;

	if (STUB_INPUTBUFFER_SIZE - m_uInputBufferPos < STUB_INPUTBUFFER_SIZE/4) {
		g_Log.SaveLog(LOG_LV_ERROR, "Stub alloc error! used too much<%d, %d>", STUB_INPUTBUFFER_SIZE, m_uInputBufferPos);
		Close();
		return;
	}

	size_t nStartPos = 0;

	MSG_ID_t uMsgID;
	MSG_LEN_t uMsgLen;

	size_t  uIDLen = sizeof(uMsgID);
	size_t	uLenLen = sizeof(uMsgLen);
	size_t	uMsgTotalLen = 0;

	while (nStartPos + uIDLen + uLenLen <= m_uInputBufferPos) {
		memcpy(&uMsgID, m_pInputBuffers + nStartPos, uIDLen);
		memcpy(&uMsgLen, m_pInputBuffers + nStartPos + uIDLen, uLenLen);

		uMsgTotalLen = uIDLen + uLenLen + uMsgLen;

		if (uMsgLen > STUB_INPUTBUFFER_SIZE / 2) {
			g_Log.SaveLog(LOG_LV_ERROR, "Stub read Error! illegality msg<%d, %d> from<%s,>", uMsgID, uMsgLen, m_sIP.c_str());
			Close();
			break;
		}

		if (nStartPos + uMsgTotalLen > m_uInputBufferPos) {
			break;
		}

		IMsgFactory* pFactory = g_MsgFactoryManager.GetFactory(uMsgID);
		if (pFactory == NULL) {
			g_Log.SaveLog(LOG_LV_WARNING, "Stub Read Warning! can not find the factory<id:%d, len:%d>", uMsgID, uMsgLen);
		} else {
			Msg* pMsg = pFactory->GetMsg();
			if (pMsg == NULL) {
				g_Log.SaveLog(LOG_LV_ERROR, "Stub Read Warning! create msg<id:%d, len:%d> failed!", uMsgID, uMsgLen);
			} else {
				pMsg->Len(uMsgLen);
				if (!pFactory->Read(pMsg, m_pInputBuffers + nStartPos + uIDLen + uLenLen, uMsgLen)) {
					g_Log.SaveLog(LOG_LV_ERROR, "Stub Read Warning! read<id:%d, len:%d> failed!", uMsgID, uMsgLen);
				} else {
					pFactory->HandleMsg(pMsg, this);
				}
			}
		}

		nStartPos += uMsgTotalLen;
	}

	assert(m_uInputBufferPos >= nStartPos);

	size_t uLeft = m_uInputBufferPos - nStartPos;
	if (uLeft > 0 && nStartPos > 0) {
		memcpy(m_pInputBuffers, m_pInputBuffers + nStartPos, uLeft);
	}
	m_uInputBufferPos = uLeft;

	//g_Log.SaveLog(LOG_LV_NORMAL, "cost time:%f", timer.Seconds());
}