#pragma once

#include <string>
using namespace std;
#include "Observer.h"

class ActionObject: public ObserverBase
{
public:
	string id;
	string shell_id;
	string cmd;
	string res;
	bool   done;

public:
	ActionObject();
	~ActionObject();

	string ToJSONString();

	static ActionObject* NewFromJsonString(const string strJson);

protected:
	virtual void OnNotify(string topic, void* data);
};

