#include "File.h"
#include <string.h>

File::File( )
{
	m_fd = -1 ;
	memset( m_szFileName, 0, _MAX_PATH ) ;
}

File::File( const char* filename, int flags )
{
	m_fd = -1 ;
	memset( m_szFileName, 0, _MAX_PATH ) ;
	Open( filename, flags ) ;
}

File::File( const char* filename, int flags, int mode )
{
	m_fd = -1 ;
	memset( m_szFileName, 0, _MAX_PATH ) ;
	Open( filename, flags, mode ) ;
}

File::~File( )
{
	if( m_fd != -1 )
	{
		Close() ;
	}
}

bool File::Open( const char* filename, int flags )
{
	strncpy( m_szFileName, filename, _MAX_PATH-1 ) ;
	m_fd = FileAPI::open_ex( filename, flags ) ;
	if( m_fd < 0 )
	{
		m_fd = -1 ;
		return false ;
	}
	return true ;
}

bool File::Open( const char* filename, int flags, int mode )
{
	strncpy( m_szFileName, filename, _MAX_PATH-1 ) ;
	m_fd = FileAPI::open_ex( filename, flags, mode ) ;
	if( m_fd < 0 )
	{
		return false ;
	}
	return true ;
}

unsigned int File::Read( void* buf, unsigned int len )
{
	return FileAPI::read_ex( m_fd, buf, len ) ;
}

unsigned int File::Write( const void* buf, unsigned int len )
{
	return FileAPI::write_ex( m_fd, buf, len ) ;
}

long File::Seek( long offset, int whence )
{
	return FileAPI::lseek_ex( m_fd, offset, whence ) ;
}

long File::Tell( )
{
	return FileAPI::tell_ex( m_fd ) ;
}

void File::Close( )
{
	FileAPI::close_ex( m_fd ) ;
}

