#pragma once

#include <string>
#include "json\json.h"

using namespace std;

struct ActionObject_T
{
	string id;
	string shell_id;
	string cmd;
	string res;

	bool done;

	ActionObject_T() : done(false)
	{	
	}

	string AsJsonString()
	{
		Json::Value root;
		root["id"] = id;
		root["cmd"] = cmd;
		root["shell"] = shell_id;
		root["res"] = res;
		root["done"] = done;
		Json::StreamWriterBuilder builder;
		return Json::writeString(builder, root);
	}

};