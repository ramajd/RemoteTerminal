// RemoteTerminal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include "mongoose.h"

#include <iostream>

using namespace std;

static const char* strPort = "8000";

#include "RequestHandler.h"


static void reqHandler(mg_connection* conn, int ev, void* p)
{
	if (ev == MG_EV_HTTP_REQUEST)
	{
		cout << "new request" << endl;

		http_message *msg = (http_message*)p;
		mg_send_head(conn, 200, msg->message.len, msg->message.p);
		mg_printf(conn, "%.*s", (int)msg->message.len, msg->message.p);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{

	RequestHandler* hdl = new RequestHandler(strPort);
	hdl->Start();
	/*mg_mgr mgr;
	mg_connection *c;

	mg_mgr_init(&mgr, NULL);
	c = mg_bind(&mgr, strPort, reqHandler);
	mg_set_protocol_http_websocket(c);

	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);
*/

	getchar();
	hdl->Stop();
	return 0;
}

