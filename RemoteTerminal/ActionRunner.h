#pragma once

#include "Observer.h"

class ActionRunner : public ObserverBase
{
public:
	ActionRunner();
	~ActionRunner();

protected:
	virtual void OnNotify(void* data);
};

