#pragma once

#include <vector>
#include <map>
#include <string>

using namespace std;

class ObserverBase
{
	friend class SubjectBase;
protected:
	virtual void OnNotify(string topic, void* data) = 0;
};

class SubjectBase
{
private:	
	map<string, vector<ObserverBase*> > m_observers;

public:
	SubjectBase() {}
	virtual ~SubjectBase() 
	{
		m_observers.clear();
	}

	virtual void AddObserver(string topic, ObserverBase* observer)
	{
		m_observers[topic].push_back(observer);
	}

	virtual void RemoveObserver(string topic, ObserverBase* observer)
	{
		if (m_observers.find(topic) != m_observers.end())
		{
			auto obslist = m_observers[topic];
			for (auto it = obslist.begin(); it != obslist.end(); it++)
			{
				if (*it = observer)
				{
					obslist.erase(it);
					break;
				}
			}
		}
	}

	void Notify(string topic, void* data)
	{
		if (m_observers.find(topic) != m_observers.end())
		{
			for (auto obs : m_observers[topic])
			{
				obs->OnNotify(topic, data);
			}
		}
	}
	
};
