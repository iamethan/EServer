#ifndef __SERVER_H__
#define __SERVER_H__

#include "Application.h"

class TestSystem;
class TestSystem2;

class Server : public App
{
public:
	Server(void);
	~Server(void);

public:
	bool Initialize();
	bool Finialize();

	void InitSystems();

private:
	TestSystem*		m_pTestSystem;
	TestSystem2*	m_pTestSystem2;
};

#endif // __SERVER_H__

