// RemoteTerminal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

using namespace std;

static const char* strPort = "8000";

#include "RequestHandler.h"


class TestClass : public ObserverBase
{
private: 
	string m_name;
public: 
	TestClass(const char* name)
	{
		m_name = string(name);
	}

	virtual void Update(void* data)
	{
		cout << m_name << ": " << (const char*)data << endl;
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	RequestHandler* hdl = new RequestHandler(strPort);
	
	TestClass t1("t1");
	TestClass t2("t2");

	hdl->Subscribe(t1);
	hdl->Subscribe(t2);
	
	hdl->Start();

	getchar();
	hdl->Stop();
	return 0;
}

