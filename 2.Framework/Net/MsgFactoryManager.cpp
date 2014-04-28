#include "MsgFactoryManager.h"
#include "Utility/GLog.h"
#include <assert.h>
#include "Stub.h"

MsgFactoryManager g_MsgFactoryManager;

MsgFactoryManager::MsgFactoryManager( ) 
{
}

MsgFactoryManager::~MsgFactoryManager( ) 
{
	MsgFactoryMap::iterator itEnd = m_mapFactories.end();
	for( MsgFactoryMap::iterator iter = m_mapFactories.begin(); iter!=itEnd; ++iter )
	{
		delete iter->second;
	}
	m_mapFactories.clear();
}

void MsgFactoryManager::AddFactory( IMsgFactory* pFactory ) 
{
	assert( pFactory );

	MSG_ID_t uMsgID = pFactory->GetMsgID();

	MSG_FACTORY* pFactoryStruct = new MSG_FACTORY;
	pFactoryStruct->m_pFactory = pFactory;
	pFactoryStruct->m_uMsgCount = 0;

	bool ret = m_mapFactories.insert( MsgFactoryMap::value_type(uMsgID, pFactoryStruct) ).second;

	if( !ret )
	{
		delete pFactoryStruct;

		g_Log.SaveLog( LOG_LV_ERROR, "PacketFactoryManager::AddFactory<%d> Error!", (int)uMsgID );
		return;
	}
}

IMsgFactory* MsgFactoryManager::GetFactory(MSG_ID_t uMsgID)
{
	MSG_FACTORY* pStruct = GetFactoryStruct( uMsgID );
	if( !pStruct )
	{
		return NULL;
	}

	return pStruct->m_pFactory;
}

MsgFactoryManager::MSG_FACTORY* MsgFactoryManager::GetFactoryStruct(MSG_ID_t uMsgID )
{
	MsgFactoryMap::iterator iter = m_mapFactories.find( uMsgID );
	if( iter == m_mapFactories.end() )
	{
		return NULL;
	}

	return iter->second;
}

Msg* MsgFactoryManager::GetMsg(MSG_ID_t uMsgID ) 
{
	MSG_FACTORY* pStruct = GetFactoryStruct( uMsgID );
	if( !pStruct )
	{
		g_Log.SaveLog( LOG_LV_WARNING, "MsgFactoryManager::CreateMsg<%d> warning! do not register the factory!",
			uMsgID );
		return NULL;
	}

	return pStruct->m_pFactory->GetMsg() ;
}

bool MsgFactoryManager::HandleMsg( Msg* pMsg, Stub* pStub )
{
	if( !pMsg || !pStub )
	{
		return false;
	}

	IMsgFactory* pFactory = GetFactory( pMsg->ID() );
	if( !pFactory )
	{
		g_Log.SaveLog( LOG_LV_WARNING, "the msg<%d> has not been declared!",
			pMsg->ID() );
		return false;
	}

	return pFactory->HandleMsg(pMsg, pStub);
}

bool MsgFactoryManager::RegisteMsgHandle(MSG_ID_t uMsgID, IMsgListener* pListener, IMsgFactory::FuncAddr pHandleFunc )
{
	IMsgFactory* pFactory = GetFactory( uMsgID );
	if( !pFactory )
	{
		g_Log.SaveLog( LOG_LV_ERROR, "register msg handle error! the msg<%d> has not been registed!",
			uMsgID );
		return false;
	}

	return pFactory->RegisterHandle( pListener, pHandleFunc );
}