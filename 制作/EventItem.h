#pragma once
#include "GameObject.h"

class EventItem : public GameObject {
public:
	//�ύX�s��
	typedef shared_ptr<GameObject>	SP;
	typedef weak_ptr<GameObject>	WP;
public:
	//�ύX��
	int number;
	bool isWork;

	virtual void Open() = 0;
	virtual void Close() = 0;
};