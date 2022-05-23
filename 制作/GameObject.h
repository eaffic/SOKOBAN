#pragma once
#include "GameEngine_Ver3_83.h"

class GameObject : public BTask {
public:
	//変更不可
	typedef shared_ptr<GameObject>	SP;
	typedef weak_ptr<GameObject>	WP;
public:
	//変更可
	#define PIXELSIZE (64)

	enum class Direction { UP, DOWN, LEFT, RIGHT };

	Direction	dir;		//向き
	ML::Vec2	pos;		//位置
	ML::Box2D	hitBase;	//当たり範囲
	ML::Vec2	moveVec;	//移動
	int			timeCnt;	//時間
	bool		canTeleport;	//瞬間移動可能
	bool		isMove;			//移動中

	GameObject() : dir(Direction::DOWN), pos(0, 0), hitBase(0, 0, 0, 0), moveVec(0, 0), timeCnt(0), canTeleport(false), isMove(false) {
	}

	~GameObject() {
	}

	bool CompareVec2(ML::Vec2 target_) {
		return ((int)this->pos.x / PIXELSIZE == (int)target_.x / PIXELSIZE) && (int)this->pos.y / PIXELSIZE == (int)target_.y / PIXELSIZE;
	}
};