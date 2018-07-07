#include "stdafx.h"
#include "ShellObject.h"

#include <iostream>


ShellObject::ShellObject(string id) : m_running(true)
{
	m_id = id;
	InitShell();

	m_thread = thread([](void* param) {
		auto instance = (ShellObject*)param;
		while (instance->m_running) 
		{
			/*if (instance->m_actionList.size() > 0) 
			{
				auto act = instance->m_actionList.front();
				instance->m_actionList.pop_front();
				instance->PerformAction(act);
			}*/
			instance->DoLoop();
			Sleep(1);
		}
	}, this);
}


ShellObject::~ShellObject()
{
	m_running = false;
	m_thread.join();
}

bool ShellObject::InitShell()
{
	m_isShellValid = false;

	SECURITY_ATTRIBUTES secAttr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	if (!CreatePipe(&m_hInputRead, &m_hInputWrite, &secAttr, 0)
		|| !CreatePipe(&m_hOutputRead, &m_hOutputWrite, &secAttr, 0))
	{
		cout << "Error creating handler pipes" << endl;
		return false;
	}

	ZeroMemory(&m_pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&m_si, sizeof(STARTUPINFO));
	m_si.cb = sizeof(m_si);	// structure size
	m_si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	m_si.wShowWindow = SW_SHOW;
	m_si.hStdInput = m_hInputRead;
	m_si.hStdOutput = m_hOutputWrite;
	m_si.hStdError = m_hOutputWrite;

	wchar_t cmd_path[1000];
	GetEnvironmentVariable(L"COMSPEC", cmd_path, 1000);

	m_isShellValid = CreateProcess(cmd_path, L"/k", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &m_si, &m_pi);
	if (m_isShellValid)
	{
		cout << "SHELL: " << m_id << " Started." << endl;
		Sleep(100);
	}
	return m_isShellValid;
}

bool ShellObject::CleanupShell()
{
	DWORD pstat;
	CloseHandle(m_pi.hThread);
	WaitForSingleObject(m_pi.hProcess, INFINITE);
	GetExitCodeProcess(m_pi.hProcess, &pstat);
	CloseHandle(m_pi.hProcess);
	m_isShellValid = false;
	cout << "SHELL: " << m_id << " Closed With: " << pstat << " code." << endl;
	return true;
}

void ShellObject::EnqueueAction(ActionObject* act)
{
	lock_guard<mutex> lguard(m_lock);
	m_actionList.push_back(act);
}

void ShellObject::PerformAction(ActionObject* act)
{
	if (!m_isShellValid)
	{
		InitShell();
	}
	string cmd = act->cmd + "\n";
	DWORD nw;
	WriteFile(m_hInputWrite, cmd.c_str(), cmd.size(), &nw, NULL);

	//Sleep(500);
	char buffer[1001];
	string res;
	while (1)
	{
		Sleep(200);
		ZeroMemory(buffer, sizeof(buffer));
		if (PeekNamedPipe(m_hOutputRead, NULL, NULL, NULL, &nw, NULL) && nw > 0)
			ReadFile(m_hOutputRead, buffer, 1000, &nw, NULL);
		if (nw > 0)
			res += string(buffer, buffer + nw);
		else
			break;
	}
	cout << "Response: (" << res.size() << ") " << res << endl;
	this->Notify(act->id, (void*)res.c_str());
}


void ShellObject::DoLoop()
{
	DWORD pstat;
	if (m_isShellValid)
	{
		GetExitCodeProcess(m_pi.hProcess, &pstat);
		if (pstat != STILL_ACTIVE)
		{
			CleanupShell();
		}
	}
	if (m_actionList.size() > 0)
	{
		auto act = m_actionList.front();
		m_actionList.pop_front();
		PerformAction(act);
	}
}
