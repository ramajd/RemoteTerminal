#pragma once

#include <microhttpd.h>

class RequestHandler
{
private:
	uint16_t	m_port;
	MHD_Daemon* m_pDeamon;

public:
	RequestHandler(uint16_t port);
	~RequestHandler();

	bool Start();
	void Stop();

protected:
	int HandleRequest(void* cls,
		MHD_Connection* connection,
		const char* url,
		const char* method,
		const char* version,
		const char* upload_data,
		size_t* upload_data_size,
		void** ptr);


	static int testme(void* cls,
		MHD_Connection* connection,
		const char* url,
		const char* method,
		const char* version,
		const char* upload_data,
		size_t * upload_data_size,
		void** ptr);
};

