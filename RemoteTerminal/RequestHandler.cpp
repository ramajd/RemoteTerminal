#include "stdafx.h"	
#include "RequestHandler.h"

#include "mongoose.h"
#include "utils.h"


RequestHandler::RequestHandler(const char* port) : m_running(false)
{
	m_portName = port;
	mg_mgr_init(&m_manager, NULL);
}


RequestHandler::~RequestHandler()
{
	this->Stop();
	mg_mgr_free(&m_manager);
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
		auto strGUID = CreateGUIDString();
		http_message hm;
		hm.message = mg_mk_str(strGUID.c_str());

		mg_send_head(conn, 200, hm.message.len, NULL);
		mg_printf(conn, "%.*s", hm.message.len, hm.message.p);

		this->Notify((void*)strGUID.c_str());
	}
}
