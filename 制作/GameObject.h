#pragma once
#include "GameEngine_Ver3_83.h"

class GameObject : public BTask {
public:
	//�ύX�s��
	typedef shared_ptr<GameObject>	SP;
	typedef weak_ptr<GameObject>	WP;
public:
	//�ύX��
	#define PIXELSIZE (64)

	enum class Direction { UP, DOWN, LEFT, RIGHT };

	Direction	dir;		//����
	ML::Vec2	pos;		//�ʒu
	ML::Box2D	hitBase;	//������͈�
	ML::Vec2	moveVec;	//�ړ�
	int			timeCnt;	//����
	bool		canTeleport;	//�u�Ԉړ��\
	bool		isMove;			//�ړ���

	GameObject() : dir(Direction::DOWN), pos(0, 0), hitBase(0, 0, 0, 0), moveVec(0, 0), timeCnt(0), canTeleport(false), isMove(false) {
	}

	~GameObject() {
	}

	bool CompareVec2(ML::Vec2 target_) {
		return ((int)this->pos.x / PIXELSIZE == (int)target_.x / PIXELSIZE) && (int)this->pos.y / PIXELSIZE == (int)target_.y / PIXELSIZE;
	}
};