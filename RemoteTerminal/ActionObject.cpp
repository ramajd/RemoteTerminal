#include "stdafx.h"
#include "ActionObject.h"

#include "json\json.h"
#include "utils.h"

ActionObject::ActionObject() : done(false)
{
}


ActionObject::~ActionObject()
{
}

string ActionObject::ToJSONString()
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

ActionObject* ActionObject::NewFromJsonString(const string strJson)
{
	Json::Value root;
	string errors;
	Json::CharReaderBuilder builder;
	Json::CharReader *reader = builder.newCharReader();
	bool ret = reader->parse(strJson.c_str(),
		strJson.c_str() + strJson.size(),
		&root,
		&errors);
	delete reader;
	if (!ret || !root.isMember("cmd"))
	{
		return NULL;
	}
	ActionObject* res = new ActionObject();
	res->id = CreateGUIDString();
	res->shell_id = root.get("shell", CreateGUIDString()).asString();
	res->cmd = root.get("cmd", "").asString();
	return res;
}

void ActionObject::OnNotify(string topic, void* data)
{
	this->res = string((char*)data);
	this->done = true;
}