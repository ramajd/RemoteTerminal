#pragma once

#include <string>
#include <thread>
#include <map>

#include "mongoose.h"
#include "json/json.h"
#include "Observer.h"
#include "application_types.h"


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

	map<string, ActionObject_T*> m_command_dict;

	Json::CharReader* m_reader;



public:
	RequestHandler(const char* port);
	~RequestHandler();

	void Start();
	void Stop();

protected:
	void HandleRequest(mg_connection* conn, int evnt, void* pdata);

	//bool ParseCommand(string data, ActionObject_T* cmd); 
	
	void HandleGetCommandStatus(mg_connection* conn, string id);
	void HandlePostCommandRequest(mg_connection* conn, string body);

	void SendResponse(mg_connection* conn, int statusCode, string response, bool isJson = false);
	void SendBadRequestResponse(mg_connection* conn);
	void SendNotFoundResponse(mg_connection* conn);
};

