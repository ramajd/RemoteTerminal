#pragma once

#include <list>

using namespace std;

class ObserverBase
{
	friend class SubjectBase;
protected:
	virtual void OnNotify(void* data) = 0;
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

	void AddObserver(ObserverBase* observer)
	{
		m_observers.push_back(observer);
	}

	void RemoveObserver(ObserverBase* observer)
	{
		for (auto it = m_observers.begin(); it != m_observers.end(); it++) 
		{
			if (*it == observer)
			{
				m_observers.remove(*it);
				break;
			}
		}
	}

	void Notify(void* data)
	{
		for (auto obs : m_observers) 
			obs->OnNotify(data);
	}
	
};
