#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

class RingBuffer
{
public:
	RingBuffer(int size = 4096);
	virtual ~RingBuffer();

	int		Read(char* buf, int count);
	int		Write(const char* buf, const int count);
	void	Empty();

	bool	IsFull();
	bool	IsEmpty();
	
	int		GetLength();
	int		GetStartPos();
	int		GetEndPos();

private:
	bool	m_bEmpty;
	bool	m_bFull;
	char*	m_pBuf;
	int		m_nBufSize;
	int		m_nReadPos;
	int		m_nWritePos;
	int		test;
};

#endif // __RING_BUFFER_H__