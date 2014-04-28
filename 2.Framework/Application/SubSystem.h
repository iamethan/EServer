#ifndef __SUB_SYSTEM_H__
#define __SUB_SYSTEM_H__

#include "Net/Msg.h"

class App;

class SubSystem : public IMsgListener
{
	friend class App;

public:
	SubSystem(void);
	~SubSystem(void);

	virtual const char*	SystemName()	= 0;
	virtual const int	SystemID()		= 0;

	virtual	bool		Initialize()	{ return true;	}
	virtual	bool		Finalize()		{ return true;	}

	virtual void		HeartBeat( int nSkip, float fTimePass) {}

private:
	bool			__Initialize();
	bool			__Finalize();
};

#endif // __SUB_SYSTEM_H__

