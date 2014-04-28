#include "SubSystem.h"

SubSystem::SubSystem(void)
{
}


SubSystem::~SubSystem(void)
{
}

bool SubSystem::__Initialize( )
{
	__InitMsgHandles();

	return Initialize( );
}

bool SubSystem::__Finalize( )
{
	bool ret = Finalize( );

	return ret;
}
