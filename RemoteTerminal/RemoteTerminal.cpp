// RemoteTerminal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>

#include "RequestHandler.h"


int _tmain(int argc, _TCHAR* argv[])
{
	RequestHandler* hndlr = new RequestHandler(8080);
	hndlr->Start();
	getchar();
	hndlr->Stop();
	return 0;
}

