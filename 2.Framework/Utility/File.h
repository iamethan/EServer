#ifndef __FILE_H__
#define __FILE_H__

#include <stdlib.h>
#include "FileAPI.h"

class File
{
public :
	
	// constructor
	File () ;
	File ( const char* filename, int flags ) ;
	File ( const char* filename, int flags, int mode ) ;
	
	// destructor
	virtual ~File () ;


	bool	Open( const char* filename, int flags ) ;
	bool	Open( const char* filename, int flags, int mode ) ;


	unsigned int	Read( void* buf, unsigned int len ) ;
	unsigned int	Write( const void* buf, unsigned int len ) ;

	long	Seek( long offset, int whence ) ;
	long	Tell( ) ;

	void	Close() ;

protected :

	int		m_fd ;
	char	m_szFileName[_MAX_PATH] ;

};

#endif
