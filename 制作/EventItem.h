#pragma once
#include "GameObject.h"

class EventItem : public GameObject {
public:
	//•ÏX•s‰Â
	typedef shared_ptr<GameObject>	SP;
	typedef weak_ptr<GameObject>	WP;
public:
	//•ÏX‰Â
	int number;
	bool isWork;

	virtual void Open() = 0;
	virtual void Close() = 0;
};