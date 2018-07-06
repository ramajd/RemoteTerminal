#pragma once

#include <list>

using namespace std;

class ObserverBase
{
public:
	virtual void Update(void* data) = 0;
};

class SubjectBase
{
private:	
	list<ObserverBase*> m_observers; 

public:
	SubjectBase() {}
	virtual ~SubjectBase() 
	{
		m_observers.clear();
	}

	void Subscribe(ObserverBase& observer)
	{
		m_observers.push_back(&observer);
	}

	void UnSubscribe(ObserverBase& observer)
	{
		for (auto it = m_observers.begin(); it != m_observers.end(); it++) 
		{
			if (*it == &observer)
			{
				m_observers.remove(*it);
				break;
			}
		}
	}

	void Notify(void* data)
	{
		for (auto obs : m_observers) 
			obs->Update(data);
	}
	
};
