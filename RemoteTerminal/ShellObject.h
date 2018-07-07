#pragma once
#include "Observer.h"
#include "ActionObject.h"

#include <Windows.h>
#include <thread>
#include <list>
#include <mutex>
using namespace std;

class ShellObject: public SubjectBase
{
private:
	string				m_id;
	list<ActionObject*> m_actionList;
	mutex				m_lock;
	thread				m_thread;
	bool				m_running;

	PROCESS_INFORMATION m_pi;
	STARTUPINFO			m_si;
	HANDLE				m_hInputRead;
	HANDLE				m_hInputWrite;
	HANDLE				m_hOutputRead;
	HANDLE				m_hOutputWrite;
	bool				m_isShellValid;


public:
	ShellObject(string id);
	~ShellObject();

	void EnqueueAction(ActionObject* act);

protected:
	bool InitShell();
	bool CleanupShell();
	void DoLoop();
	void PerformAction(ActionObject* act);

};

