#ifndef __OBJPOOL_H__
#define __OBJPOOL_H__

#include <assert.h>
#include "GameUtil.h"

template<class T>
class ObjPool
{
public:
	ObjPool()
	{
		m_papObj	= NULL;
		m_nMaxCount		= -1;
		m_nPosition		= -1;
	}

	~ObjPool()
	{
		Term() ;

		assert(m_papObj == NULL);
	}

	bool Init(int nMaxCount)
	{
		assert( nMaxCount > 0 );
		if ( nMaxCount <= 0 )
			return false;

		m_nMaxCount		= nMaxCount;
		m_nPosition		= 0;
		m_papObj		= new T* [m_nMaxCount];

		int i;
		for( i = 0; i < m_nMaxCount; i++ )
		{
			m_papObj[i] = new T;
			if ( m_papObj[i] == NULL )
			{
				assert( m_papObj[i] != NULL );
				return false;
			}
		}

		return true;
	}

	void Term()
	{
		if ( m_papObj != NULL )
		{
			int i;
			for ( i = 0; i < m_nMaxCount; i++ )
			{
				SAFE_DELETE( m_papObj[i] );
			}

			SAFE_DELETE_ARRAY(m_papObj);
			m_papObj = NULL;
		}

		m_nMaxCount		= -1;
		m_nPosition		= -1;
	}

	T* NewObj()
	{
		assert( m_nPosition < m_nMaxCount );
		if ( m_nPosition >= m_nMaxCount )
		{
			return NULL;
		}

		T *pObj = m_papObj[m_nPosition];
		pObj->SetPoolID( (unsigned int)m_nPosition );
		m_nPosition++;

		return pObj;
	}

	void DeleteObj( T *pObj )
	{
		assert( pObj != NULL );	
		if ( pObj == NULL )
		{
			return ;
		}

		assert( m_nPosition > 0 );	
		if ( m_nPosition <= 0 )
		{
			return ;
		}

		unsigned int uDelIndex = pObj->GetPoolID();
		assert(uDelIndex < (unsigned int)m_nPosition );
		if (uDelIndex >= (unsigned int)m_nPosition )
		{
			return;
		}


		m_nPosition--;
		T *pDelObj			= m_papObj[uDelIndex];
		m_papObj[uDelIndex]	= m_papObj[m_nPosition];
		m_papObj[m_nPosition]	= pDelObj;

		m_papObj[uDelIndex]->SetPoolID(uDelIndex );
		m_papObj[m_nPosition]->SetPoolID( INVALID_ID );

		return;
	}

	int GetCount()const
	{
		return m_nPosition;
	}

private:
	T				**m_papObj;
	int				m_nMaxCount;
	int				m_nPosition;
};

#endif
