#include "RingBuffer.h"
#include <assert.h>
#include <memory.h>

// ����  
RingBuffer::RingBuffer(int size)
{  
	m_nBufSize = size;  
	m_nReadPos = 0;  
	m_nWritePos = 0;  
	m_pBuf = new char[m_nBufSize];
	m_bEmpty = true;  
	m_bFull = false; 
	test = 0;
}  
RingBuffer::~RingBuffer()  
{  
	delete[] m_pBuf;  
}  

/************************************************************************/  
/* �򻺳���д�����ݣ�����ʵ��д����ֽ���                               */  
/************************************************************************/  
int RingBuffer::Write(const char* buf, const int count)  
{  
	if(count <= 0) 
		return 0;  
	m_bEmpty = false;  
	// ���������������ܼ���д��  
	if(m_bFull)   
	{  
		return 0;  
	}  
	else if(m_nReadPos == m_nWritePos) // ������Ϊ��ʱ  
	{  
		/*                          == �ڴ�ģ�� == 
		(empty)             m_nReadPos                (empty)                       
		|----------------------------------|-----------------------------------------| 
		m_nWritePos        m_nBufSize 
		*/  
		int leftcount = m_nBufSize - m_nWritePos;  
		if(leftcount > count)  
		{  
			memcpy(m_pBuf + m_nWritePos, buf, count);  
			m_nWritePos += count;  
			m_bFull = (m_nWritePos == m_nReadPos);  
			return count;  
		}  
		else  
		{  
			memcpy(m_pBuf + m_nWritePos, buf, leftcount);  
			m_nWritePos = (m_nReadPos > count - leftcount) ? count - leftcount : m_nWritePos;  
			memcpy(m_pBuf, buf + leftcount, m_nWritePos);  
			m_bFull = (m_nWritePos == m_nReadPos);  
			return leftcount + m_nWritePos;  
		}  
	}   
	else if(m_nReadPos < m_nWritePos) // ��ʣ��ռ��д��  
	{  
		/*                           == �ڴ�ģ�� == 
		(empty)                 (data)                     (empty) 
		|-------------------|----------------------------|---------------------------| 
		m_nReadPos                m_nWritePos       (leftcount)            
		*/  
		// ʣ�໺������С(��д��λ�õ�������β)  

		int leftcount = m_nBufSize - m_nWritePos;
		int test = m_nWritePos;
		if(leftcount > count)   // ���㹻��ʣ��ռ���  
		{  
			memcpy(m_pBuf + m_nWritePos, buf, count);  
			m_nWritePos += count;  
			m_bFull = (m_nReadPos == m_nWritePos);  
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize);      
			return count;  
		}  
		else       // ʣ��ռ䲻��  
		{  
			// �������ʣ��ռ䣬�ٻ�ͷ�ҿռ���  
			memcpy(m_pBuf + test, buf, leftcount);  

			m_nWritePos = (m_nReadPos >= count - leftcount) ? count - leftcount : m_nReadPos;  
			memcpy(m_pBuf, buf + leftcount, m_nWritePos);  
			m_bFull = (m_nReadPos == m_nWritePos);     
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize);   
			return leftcount + m_nWritePos;  
		}  
	}  
	else  
	{  
		/*                          == �ڴ�ģ�� == 
		(unread)                 (read)                     (unread) 
		|-------------------|----------------------------|---------------------------| 
		m_nWritePos    (leftcount)    m_nReadPos                       
		*/  
		int leftcount = m_nReadPos - m_nWritePos;  
		if(leftcount > count)  
		{  
			// ���㹻��ʣ��ռ���  
			memcpy(m_pBuf + m_nWritePos, buf, count);  
			m_nWritePos += count;  
			m_bFull = (m_nReadPos == m_nWritePos);  
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize);   
			return count;  
		}  
		else  
		{  
			// ʣ��ռ䲻��ʱҪ�������������  
			memcpy(m_pBuf + m_nWritePos, buf, leftcount);  
			m_nWritePos += leftcount;  
			m_bFull = (m_nReadPos == m_nWritePos);  
			assert(m_bFull);  
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize);     
			return leftcount;  
		}  
	}  
}  
/************************************************************************/  
/* �ӻ����������ݣ�����ʵ�ʶ�ȡ���ֽ���                                 */  
/************************************************************************/  
int RingBuffer::Read(char* buf, int count)  
{  
	if(count <= 0) 
		return 0;  
	m_bFull = false;  
	if(m_bEmpty)       // �������գ����ܼ�����ȡ����  
	{  
		return 0;  
	}  
	else if(m_nReadPos == m_nWritePos)   // ��������ʱ  
	{  
		/*                          == �ڴ�ģ�� == 
		(data)          m_nReadPos                (data)     
		|--------------------------------|--------------------------------------------| 
		m_nWritePos         m_nBufSize 
		*/  
		int leftcount = m_nBufSize - m_nReadPos;  
		if(leftcount > count)  
		{  
			memcpy(buf, m_pBuf + m_nReadPos, count);  
			m_nReadPos += count;  
			m_bEmpty = (m_nReadPos == m_nWritePos);  
			return count;  
		}  
		else  
		{  
			memcpy(buf, m_pBuf + m_nReadPos, leftcount);  
			m_nReadPos = (m_nWritePos > count - leftcount) ? count - leftcount : m_nWritePos;  
			memcpy(buf + leftcount, m_pBuf, m_nReadPos);  
			m_bEmpty = (m_nReadPos == m_nWritePos);  
			return leftcount + m_nReadPos;  
		}  
	}  
	else if(m_nReadPos < m_nWritePos)   // дָ����ǰ(δ�����������ӵ�)  
	{  
		/*                          == �ڴ�ģ�� == 
		(read)                 (unread)                      (read)     
		|-------------------|----------------------------|---------------------------| 
		m_nReadPos                m_nWritePos                     m_nBufSize 
		*/  
		int leftcount = m_nWritePos - m_nReadPos;  
		int c = (leftcount > count) ? count : leftcount;  
		memcpy(buf, m_pBuf + m_nReadPos, c);  
		m_nReadPos += c;  
		m_bEmpty = (m_nReadPos == m_nWritePos);  
		assert(m_nReadPos <= m_nBufSize);  
		assert(m_nWritePos <= m_nBufSize);  
		return c;  
	}  
	else          // ��ָ����ǰ(δ�����ݿ����ǲ����ӵ�)  
	{  
		/*                          == �ڴ�ģ�� == 
		(unread)                (read)                      (unread) 
		|-------------------|----------------------------|---------------------------| 
		m_nWritePos                  m_nReadPos                  m_nBufSize 

		*/  
		int leftcount = m_nBufSize - m_nReadPos;  
		if(leftcount > count)   // δ������������ֱ�Ӷ�ȡ����  
		{  
			memcpy(buf, m_pBuf + m_nReadPos, count);  
			m_nReadPos += count;  
			m_bEmpty = (m_nReadPos == m_nWritePos);  
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize);  
			return count;  
		}  
		else       // δ�����������㣬��ص�������ͷ��ʼ��  
		{  
			memcpy(buf, m_pBuf + m_nReadPos, leftcount);  
			m_nReadPos = (m_nWritePos >= count - leftcount) ? count - leftcount : m_nWritePos;  
			memcpy(buf + leftcount, m_pBuf, m_nReadPos);  
			m_bEmpty = (m_nReadPos == m_nWritePos);  
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize);  
			return leftcount + m_nReadPos;  
		}    
	}  
}  
/************************************************************************/  
/* ��ȡ��������Ч���ݳ���                                               */  
/************************************************************************/  
int RingBuffer::GetLength()  
{  
	if(m_bEmpty)  
	{  
		return 0;  
	}  
	else if(m_bFull)  
	{  
		return m_nBufSize;  
	}  
	else if(m_nReadPos < m_nWritePos)  
	{  
		return m_nWritePos - m_nReadPos;  
	}  
	else  
	{  
		return m_nBufSize - m_nReadPos + m_nWritePos;  
	}  
}  
void RingBuffer::Empty()  
{  
	m_nReadPos = 0;  
	m_nWritePos = 0;  
	m_bEmpty = true;  
	m_bFull = false;  
} 

bool RingBuffer::IsEmpty()  
{  
	return m_bEmpty;  
}  

bool RingBuffer::IsFull()  
{  
	return m_bFull;  
}  

int RingBuffer::GetStartPos()
{
	return m_nReadPos;
}

int RingBuffer::GetEndPos()
{
	return m_nWritePos;
}
