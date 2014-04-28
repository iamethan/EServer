#ifndef __GAMEUTIL_H__
#define __GAMEUTIL_H__

#define __PI			3.1415f
#define __HALF_PI		__PI / 2
#define __QUARTER_PI	__PI / 4

///////////////////////////////////////////////////////////////////////
//���������Ժ�������
///////////////////////////////////////////////////////////////////////

//��ǰ�̹߳���һ��ʱ��
extern void			MySleep(unsigned int millionseconds=0 ) ;

#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if ((x)!=NULL) { delete (x); (x)=NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if ((x)!=NULL) { delete[] (x); (x)=NULL; }
#endif

#endif
