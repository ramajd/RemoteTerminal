#include "stdafx.h"	
#include "RequestHandler.h"

#include "mongoose.h"
#include "json/json.h"
#include "utils.h"


RequestHandler::RequestHandler(const char* port) : m_running(false)
{
	m_portName = port;
	mg_mgr_init(&m_manager, NULL);

	Json::CharReaderBuilder builder;
	m_reader = builder.newCharReader();
}


RequestHandler::~RequestHandler()
{
	this->Stop();
	mg_mgr_free(&m_manager);
	
	delete m_reader;
	for (auto cmd : m_command_dict)
	{
		if (cmd.second != NULL)
			delete cmd.second;
	}
	m_command_dict.clear();
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
			//cout << "ID: " << id << endl;
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
	Json::Value root;
	std::string errors;
	ActionObject_T* cmd = NULL;

	if (!m_reader->parse(body.c_str(), body.c_str() + body.size(), &root, &errors))
	{
		cout << "ERROR: " << __FUNCTION__ << " - " << errors << endl;
		SendBadRequestResponse(conn);
	}
	else if (!root.isMember("cmd"))
	{
		cout << "ERROR: " << __FUNCTION__ << " - cmd param not found" << endl;
		SendBadRequestResponse(conn);
	}
	else
	{
		cmd = new ActionObject_T();
		cmd->id = CreateGUIDString();
		cmd->cmd = root.get("cmd", "").asString();
		cmd->shell_id = root.get("shell", CreateGUIDString()).asString();
		SendResponse(conn, 200, cmd->AsJsonString(), true);
		m_command_dict[cmd->id] = cmd;
		this->Notify(cmd);
	}
}

void RequestHandler::HandleGetCommandStatus(mg_connection* conn, string id)
{
	if (m_command_dict.find(id) != m_command_dict.end())
	{
		SendResponse(conn, 200, m_command_dict[id]->AsJsonString(), true);
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