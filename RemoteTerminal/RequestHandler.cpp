#include "stdafx.h"	
#include "RequestHandler.h"

#include <iostream>
#include "mongoose.h"


RequestHandler::RequestHandler(const char* port) : m_running(false)
{
	m_portName = port;
	mg_mgr_init(&m_manager, NULL);
}


RequestHandler::~RequestHandler()
{
	this->Stop();
	mg_mgr_free(&m_manager);
	
	for (auto act : m_actionList)
	{
		if (act.second != NULL)
			delete act.second;
	}
	m_actionList.clear();
}


void RequestHandler::Start()
{
	m_connection = mg_bind(&m_manager, m_portName.c_str(), [](mg_connection *c, int ev, void* evdata) {
		auto instance = static_cast<RequestHandler*>(c->user_data);
		instance->HandleRequest(c, ev, evdata);
	});
	m_connection->user_data = (void*)this;
	mg_set_protocol_http_websocket(m_connection);
	m_running = true;

	m_thread = thread([](void* param){
		auto instance = static_cast<RequestHandler*>(param);
		while (instance->m_running)
		{
			mg_mgr_poll(&instance->m_manager, 1000);
		}
	}, this);
}

void RequestHandler::Stop()
{
	m_running = false;
	m_thread.join();
}

void RequestHandler::HandleRequest(mg_connection* conn, int evnt, void* evtdata)
{
	if (evnt == MG_EV_HTTP_REQUEST)
	{
		auto req = (struct http_message *) evtdata;
		char method[20] = { 0x00 };
		sprintf_s(method, "%.*s", req->method.len, req->method.p);

		if (strcmp(method, "GET") == 0)
		{
			cout << "NEW GET: ";
			char id[100] = { 0x00 };
			mg_get_http_var(&req->query_string, "id", id, 100);
			HandleGetCommandStatus(conn, id);
			cout << "ID: " << id << endl;
		}
		else if (strcmp(method, "POST") == 0)
		{
			cout << "NEW POST: " << endl;
			char body[200] = { 0x00 };
			sprintf_s(body, "%.*s", req->body.len, req->body.p);
			HandlePostCommandRequest(conn, body);
		}
		else
		{
			cout << "INVALID METHOD: " << method << endl;
			SendBadRequestResponse(conn);
		}
	}
}



void RequestHandler::HandlePostCommandRequest(mg_connection* conn, string body)
{
	ActionObject* act = ActionObject::NewFromJsonString(body);
	if (act != NULL)
	{
		SendResponse(conn, 200, act->ToJSONString(), true);
		m_actionList[act->id] = act;
		this->Notify(act);
	}
	else
	{
		SendBadRequestResponse(conn);
	}
}

void RequestHandler::HandleGetCommandStatus(mg_connection* conn, string id)
{
	if (m_actionList.find(id) != m_actionList.end())
	{
		SendResponse(conn, 200, m_actionList[id]->ToJSONString(), true);
	}
	else
	{
		SendNotFoundResponse(conn);
	}
}

void RequestHandler::SendResponse(mg_connection* conn, int statusCode, string response, bool isJson)
{
	http_message msg;
	msg.message = mg_mk_str(response.c_str());
	if (isJson)
		mg_send_head(conn, statusCode, msg.message.len, "Content-Type: application/json");
	else 
		mg_send_head(conn, statusCode, msg.message.len, NULL);
	mg_printf(conn, "%.*s", msg.message.len, msg.message.p);
}

void RequestHandler::SendBadRequestResponse(mg_connection* conn)
{
	SendResponse(conn, 400, "Bad Request");
}

void RequestHandler::SendNotFoundResponse(mg_connection* conn)
{
	SendResponse(conn, 400, "Not Found");
}


void RequestHandler::AddObserver(ObserverBase* observer)
{
	SubjectBase::AddObserver("new_action", observer);
}
void RequestHandler::Notify(void* data)
{
	SubjectBase::Notify("new_action", data);
}
