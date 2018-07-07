#include "stdafx.h"
#include "ActionRunner.h"

#include <Windows.h>
#include <iostream>


ActionRunner::ActionRunner() : m_running(true)
{
	m_actionThread = thread([](void* param) {
		auto instance = static_cast<ActionRunner*>(param);
		instance->ManageActions();
	}, this);
}


ActionRunner::~ActionRunner()
{
	m_running = false;
	m_actionThread.join();
}


void ActionRunner::OnNotify(string topic, void* data)
{
	auto obj = (ActionObject*)data;
	if (obj != NULL)
	{
		m_actionList.push_back(obj);
		cout << "New Action Added: " << obj->id << endl;
	}
}

void ActionRunner::ManageActions()
{
	while (m_running)
	{
		auto act = FetchAction();
		if (act != NULL)
		{
			ExecuteAction(act);
		}
		Sleep(1);
	}
}

ActionObject* ActionRunner::FetchAction()
{
	ActionObject* act = NULL;
	if (m_actionList.size() > 0)
	{
		act = m_actionList.front();
		m_actionList.pop_front();
	}
	return act;
}

void ActionRunner::ExecuteAction(ActionObject* act)
{
	string shellId = act->shell_id;
	string actId = act->id;

	if (m_shellDict.find(act->shell_id) == m_shellDict.end() || m_shellDict[act->shell_id] == NULL)
	{
		m_shellDict[act->shell_id] = new ShellObject(shellId);
	}
	m_shellDict[shellId]->AddObserver(actId, act);
	m_shellDict[shellId]->EnqueueAction(act);
}
