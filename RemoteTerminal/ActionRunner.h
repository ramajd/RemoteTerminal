#pragma once

#include <list>
#include <map>
#include <mutex>
#include <thread>
using namespace std;

#include "Observer.h"
#include "ActionObject.h"
#include "ShellObject.h"

class ActionRunner : public ObserverBase
{
private:
	thread				      m_actionThread;
	bool				      m_running;
	list<ActionObject*>		  m_actionList;
	map<string, ShellObject*> m_shellDict;
public:
	ActionRunner();
	~ActionRunner();

protected:
	virtual void OnNotify(string topic, void* data);

	void ManageActions();
	ActionObject* FetchAction();
	void ExecuteAction(ActionObject* act);
};

