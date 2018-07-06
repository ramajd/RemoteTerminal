#pragma once

#include <string>
#include <thread>

#include "mongoose.h"

using namespace std;

class RequestHandler
{
private: 
	string			m_portName;
	mg_mgr			m_manager;
	mg_connection*	m_connection;
	thread			m_thread;
	bool			m_running;

public:
	RequestHandler(const char* port);
	~RequestHandler();

	void Start();
	void Stop();

protected:
	void HandleRequest(mg_connection* conn, int evnt, void* pdata);
};

