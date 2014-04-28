#ifndef  __ID_GENERATOR_H__
#define  __ID_GENERATOR_H__

#include <set>
#include <assert.h>
#include "GLog.h"

template <class T> 
class IDGenerator {
public:
	//will generate ID from [tStart, tEnd]
	IDGenerator(   const T tStart, 
		const T tEnd, 
		const T tNotAvailible);
	~IDGenerator();

	T GetUnusedID();
	T GetMaxUsedID() { 
		if( m_tStart == m_tNextAvailibleID) {
			return m_tNotAvailible;
		}
		else {
			return m_tNextAvailibleID-1;
		}
	}

	bool  IsIDUsed( const T tID );
	bool  RecycleID( const T tParam1 );
	bool  UseID( const T tParam1 );

protected:   
	std::set<T, std::less<T> >   m_setUnusedID;
	typedef typename std::set<T, std::less<T> >::iterator  _Set_It;
	typedef typename std::set<T, std::less<T> >::const_iterator  _Set_Con_It;

	T   m_tNextAvailibleID;
	T   m_tStart;
	T   m_tEnd;
	T   m_tNotAvailible;
};

template <class T>
IDGenerator<T>::IDGenerator( const T tStart, 
	const T tEnd, 
	const T tNotAvalilble)
	:  m_tNextAvailibleID( tStart ),
	m_tStart( tStart ),
	m_tEnd( tEnd ),
	m_tNotAvailible( tNotAvalilble )
{

}

template<class T>
IDGenerator<T>::~IDGenerator()
{
	_Set_It iter = m_setUnusedID.begin();
	_Set_It iterEnd = m_setUnusedID.end();
	_Set_It iterTmp;
	while( iter != iterEnd ) {
		iterTmp = iter++;
		m_setUnusedID.erase(iterTmp);      
	}
}

template <class T> 
bool IDGenerator<T>::IsIDUsed( const T t )
{
	if( t >= m_tNextAvailibleID ) {
		return false;
	}
	else {
		//check for unused ID
		if( m_setUnusedID.find( t ) == m_setUnusedID.end() ) {
			return false;
		}
		else {
			return true;
		}
	}

	return false;
}

//use a ID
template <class T> 
bool IDGenerator<T>::UseID( const T tParam1 )
{
	if( tParam1 > m_tEnd || tParam1 < m_tStart || tParam1 == m_tNotAvailible || tParam1 > m_tEnd ) {
		g_Log.SaveLog( LOG_LV_ERROR, "IDGenerator<T>::UseID() fail!" );
		return false;
	}

	if( tParam1 == m_tNextAvailibleID ) {
		m_tNextAvailibleID++;
		return true;
	}
	else if( tParam1 > m_tNextAvailibleID ) {
		assert( m_setUnusedID.find( tParam1 ) == m_setUnusedID.end() );
		T i;
		for( i=m_tNextAvailibleID; i<tParam1;i++ ) {
			m_setUnusedID.insert(i);
		}
		assert( tParam1 < m_tEnd );
		m_tNextAvailibleID = tParam1+1;
		return true;
	}
	else {
		_Set_It it;
		//find the id in unused id set
		if( (it = m_setUnusedID.find( tParam1 )) != m_setUnusedID.end() ) {
			m_setUnusedID.erase( it );
			return true;
		}
		else {
			return false;  
		}
	}

	return false;
}

template <class T>
T IDGenerator<T>::GetUnusedID()
{
	//is next id used up?
	if( m_tNextAvailibleID > m_tEnd ) {
		if( m_setUnusedID.empty() ) {
			g_Log.SaveLog( LOG_LV_ERROR, "IDGenerator::GetUnusedID() All ID has been used up" );
			return m_tNotAvailible;
		}
		else { //use recycled id
			_Set_It iter = m_setUnusedID.begin();
			T tReturn = *iter;
			m_setUnusedID.erase(iter);
			return tReturn;
		}
	}

	//no, we get next availible id
	T tReturn = m_tNextAvailibleID;
	m_tNextAvailibleID++;
	return tReturn;
}

template <class T>
bool IDGenerator<T>::RecycleID( const T tParam1 )
{
	m_setUnusedID.insert( tParam1 );
	return true;
}

#endif // __ID_GENERATOR_H__


