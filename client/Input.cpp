#include "Input.h"

bool Mouse::instanceFlag = false;
Mouse* Mouse::single = NULL;

Mouse::Mouse()
{
	
}

Mouse* Mouse::getInstance()
{
	if (!instanceFlag)
	{
		single = new Mouse();
		instanceFlag = true;
		return single;
	}
	else
		return single;
}
