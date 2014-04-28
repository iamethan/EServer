#ifndef __MSG_DEFINE_H__
#define __MSG_DEFINE_H__

#include "Net.h"

#include <string>
#include <vector>

enum MSGID
{
	MSGID_Server = 10,
	MSGID_Client,
};

DECLEAR_MSG_BEGIN(ServerMsg, MSGID_Server)
	struct PacketBody
	{
		int m_a;
		int m_b;
		std::string m_s;

		MSGPACK_DEFINE(m_a, m_b, m_s);
	};

	PacketBody body;    // 包体
DECLEAR_MSG_END;

DECLEAR_MSG_BEGIN(ClientMsg, MSGID_Client)
	struct PacketBody
	{
		std::vector<int>	m_v;
		int					m_a;

		MSGPACK_DEFINE(m_v, m_a);
	};

	PacketBody body;    // 包体
DECLEAR_MSG_END;


#endif // __MSG_DEFINE_H__