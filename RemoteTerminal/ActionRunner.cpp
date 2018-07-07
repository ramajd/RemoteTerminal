#include "stdafx.h"
#include "ActionRunner.h"
#include "application_types.h"

#include <iostream>


ActionRunner::ActionRunner()
{
}


ActionRunner::~ActionRunner()
{
}


void ActionRunner::OnNotify(void* data)
{
	auto obj = (ActionObject_T*)data;
	if (obj != NULL)
	{
		cout << __FUNCTION__ << endl << obj->AsJsonString() << endl;
	}
}