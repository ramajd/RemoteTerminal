#include "stdafx.h"
#include "RequestHandler.h"

#include <iostream>
#include <functional>

#define PAGE	""	\
	"<html>" \
	"<head><title>Remote Terminal API Docs</title></head>" \
	"<body>" \
	"	<h1>Remote Terminal API Docs:</h1>" \
	"	<hr>" \
	"	<ul>" \
	"		<li> [POST] /exec</li>" \
	"		" \
	"		" \
	"		" \
	"	</ul>" \
	"</body>" \
	"</html>"


RequestHandler::RequestHandler(uint16_t port)
{
	m_port = port;
}


RequestHandler::~RequestHandler()
{
}

bool RequestHandler::Start()
{
	this->Stop();
	m_pDeamon = MHD_start_daemon(
		MHD_USE_THREAD_PER_CONNECTION,
		m_port,
		NULL, NULL,
		[](void* cls, MHD_Connection* conn, const char* url, const char* method, const char* version,
			const char* upload_data, size_t* upload_data_size, void** con_cls) -> int {
			auto self = (RequestHandler*)cls;
			return self->HandleRequest(cls, conn, url, method, version, upload_data, upload_data_size, con_cls);
		},
		(void*)this,
		MHD_OPTION_END);
	return m_pDeamon != NULL;
}

void RequestHandler::Stop()
{
	if (m_pDeamon != NULL)
	{
		MHD_stop_daemon(m_pDeamon);
		m_pDeamon = NULL;
	}
}

int RequestHandler::HandleRequest(void* cls,
	MHD_Connection* connection,
	const char* url,
	const char* method,
	const char* version,
	const char* upload_data,
	size_t * upload_data_size,
	void** ptr)
{
	static int dummy;
	const char* page = "WELCOME TO TEST";
	MHD_Response* response; 
	int ret;

	std::cout << "NEW [" << method << "] From [" << url << "] Version ["
		<< version << "]" << std::endl;

	const MHD_ConnectionInfo *inf = MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CLIENT_ADDRESS);
	std::cout << "\t - " << inet_ntoa(((sockaddr_in*)inf->client_addr)->sin_addr) << std::endl;


	MHD_get_connection_values(connection, MHD_HEADER_KIND, [](void* cls, enum MHD_ValueKind kind, const char* key, const char* val) ->int {
		std::cout << "\t - " << key << " -> " << val << std::endl;
		return MHD_YES;
	}, NULL);

	if (strcmp(method, "GET") == 0)
	{
		if (&dummy != *ptr)
		{
			*ptr = &dummy;
			return MHD_YES;
		}
		if (*upload_data_size != 0)
		{
			return MHD_NO;
		}
		*ptr = NULL;
		response = MHD_create_response_from_buffer(strlen(PAGE),
			(void*)PAGE, MHD_RESPMEM_PERSISTENT);
		ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
		MHD_destroy_response(response);
		return ret;
	}
	return MHD_NO;

}
