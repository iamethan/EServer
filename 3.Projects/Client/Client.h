#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "Application.h"

class TestSystem;

class Client : public App
{
public:
	Client(void);
	~Client(void);

public:
	bool Initialize();
	bool Finialize();

	void InitSystems();

private:
	TestSystem*		m_pTestSystem;
};

#endif // __CLIENT_H__

