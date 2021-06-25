#pragma once

#include <list>

class Observable;
class Observer;

class Observer
{
	friend class Observable;
public:
	virtual void OnChange() = 0;
	virtual ~Observer();
private:
	void ChangeObserving(Observable* obervable);
private:
	Observable* _oberserving = nullptr;
};

class Observable
{
public:
	virtual void UnSubscribe(Observer* observer);
	virtual void Subscribe(Observer* observer);
	virtual void CallOnChange();
private:
	std::list<Observer*> _observers;
};