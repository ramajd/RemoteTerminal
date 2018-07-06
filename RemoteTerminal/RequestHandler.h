#pragma once

#include <string>
#include <thread>

#include "mongoose.h"
#include "Observer.h"

using namespace std;

struct mg_mgr;
struct mg_connection;


class RequestHandler: public SubjectBase
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

