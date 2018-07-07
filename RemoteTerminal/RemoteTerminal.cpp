// RemoteTerminal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "RequestHandler.h"
#include "ActionRunner.h"

using namespace std;

static const char* strPort = "8000";

int _tmain(int argc, _TCHAR* argv[])
{
	auto requestHandler = new RequestHandler(strPort);
	auto actionRunner = new ActionRunner();

	requestHandler->AddObserver(actionRunner);

	
	requestHandler->Start();

	getchar();
	
	requestHandler->Stop();
	
	return 0;
}

