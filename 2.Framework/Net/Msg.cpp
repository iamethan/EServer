#include "Msg.h"
#include "Utility/GLog.h"
#include <assert.h>

bool IMsgFactory::RegisterHandle(IMsgListener* pListener, FuncAddr pFuncHandle)
{
	HandleList::iterator itEnd = m_lstHandles.end();
	for( HandleList::iterator it = m_lstHandles.begin(); it != itEnd; ++it )
	{
		if( (*it)->m_pListener == pListener )
		{
			g_Log.SaveLog(LOG_LV_WARNING, "register event handle warning! the listener<%s> has been registed!",
				pListener->SystemName() );
			return false;
		}
	}

	Handle* pHandle = new Handle;
	pHandle->m_pListener = pListener;
	pHandle->m_pFuncHandle = pFuncHandle;

	m_lstHandles.push_back( pHandle );

	return true;
}

bool IMsgFactory::HandleMsg(Msg* pMsg, Stub* pStub)
{
	HandleList::iterator itEnd = m_lstHandles.end();
	for( IMsgFactory::HandleList::iterator it = m_lstHandles.begin(); it != itEnd; ++it )
	{
		IMsgFactory::Handle* pHandle = *it;
		assert( pHandle );

		if( !(pHandle->m_pListener->__HandleMsg(pMsg, pStub, pHandle->m_pFuncHandle) ) )
		{
			g_Log.SaveLog( LOG_LV_ERROR, "sys<%s> handle packet<%d> error!",
				pHandle->m_pListener->SystemName(), pMsg->ID() );
		}
	}

	return true;
}