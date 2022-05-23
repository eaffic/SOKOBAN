//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Box.h"
#include  "Task_Player.h"

namespace  Box
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgBox = DG::Image::Create("./data/image/map/TX Props.png");
		this->sdMove = DM::Sound::CreateSE("./data/audio/boxmove.wav");
		this->sdWater = DM::Sound::CreateSE("./data/audio/boxwater.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgBox.reset();
		this->sdMove.reset();
		this->sdWater.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 0.45f;

		this->moveSpeed = 0;
		this->stopTime = 15;

		this->isMove = false;
		this->canTeleport = false;
		this->inWater = false;
		this->inGoal = false;
		this->seplay = false;

		this->pos = ML::Vec2(PIXELSIZE / 2, PIXELSIZE / 2);
		this->est = ML::Vec2(0, 0);
		this->hitBase = ML::Box2D(-PIXELSIZE / 2, -PIXELSIZE / 2, PIXELSIZE, PIXELSIZE);

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->RenderSet();

		//�ړ��������ݒ�
		if (this->isMove) {
			this->Move();
			if (!this->seplay) {
				this->res->sdMove->Play_Normal(true);
				this->seplay = true;
			}
			this->stopTime = 15;
		}
		else {
			for (auto box = ge->qa_Boxs->begin(); box != ge->qa_Boxs->end(); ++box) {
				if ((*box)->stopTime < 0 && (*box)->seplay) {
					this->res->sdMove->Stop();
					this->seplay = false;
				}
			}
			this->stopTime--;
		}

		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(-32, -96, 64, 128);
		draw.Offset(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		ML::Box2D src(160, 0, 32, 64);
		if (this->inWater) {
			draw.y -= PIXELSIZE / 4;
			draw.h -= PIXELSIZE / 2;
			src.h -= PIXELSIZE / 2;
			this->res->imgBox->Draw(draw, src, ML::Color(1.0f, 0.7f, 0.7f, 1.0f));
		}
		else if (this->inGoal) {
			this->res->imgBox->Draw(draw, src, ML::Color(1.0f, 1.0f, 0.7f, 0.7f));
		}
		else {
			this->res->imgBox->Draw(draw, src);
		}
	}

	//-------------------------------------------------------------------
	//�`�惌�C���[����
	void Object::RenderSet() {
		if (this->inWater) return;

		//�㉺�͑��̔�������̏ꍇ
		for (auto it = ge->qa_Boxs->begin(); it != ge->qa_Boxs->end(); ++it) {
			if ((*it)->pos != this->pos && !(*it)->inWater && this->number > (*it)->number) {
				int sx = (int)(*it)->pos.x / PIXELSIZE - (int)this->pos.x / PIXELSIZE;
				int sy = (int)(*it)->pos.y / PIXELSIZE - (int)this->pos.y / PIXELSIZE;
				if ( sx == 0 && (sy <= 1 || sy >= -1)) {
					if (sy > 0 && this->render2D_Priority[1] < (*it)->render2D_Priority[1]) {
						auto temp = this->render2D_Priority[1];
						this->render2D_Priority[1] = (*it)->render2D_Priority[1];
						(*it)->render2D_Priority[1] = temp;
					}
					else if (sy < 0 && this->render2D_Priority[1] > (*it)->render2D_Priority[1]) {
						auto temp = this->render2D_Priority[1];
						this->render2D_Priority[1] = (*it)->render2D_Priority[1];
						(*it)->render2D_Priority[1] = temp;
					}
				}
			}
		}

		//���ɗ������ꍇ
		if (!this->isMove) {
			if (ge->qa_Map->arrMap[ge->qa_Map->WATERBARRIER][(int)this->pos.y / PIXELSIZE][(int)this->pos.x / PIXELSIZE] >= 0) {
				ge->qa_Map->arrMap[ge->qa_Map->WATERBARRIER][(int)this->pos.y / PIXELSIZE][(int)this->pos.x / PIXELSIZE] = -1;
				this->inWater = true;
				this->render2D_Priority[1] += 0.2f;
				this->AddForce(ML::Vec2(0, 36), 6, Direction::DOWN);
				this->res->sdWater->Play_Normal(false);
			}
		}

		//�S�[���ɂ���ꍇ
		auto goalPos = ge->qa_MapObject->goalBoxPos;
		for (int i = 0; i < 20; ++i) {
			if (goalPos[i].x == -1) break;
			if (this->CompareVec2(goalPos[i])) {
				this->inGoal = true;
				return;
			}
			else {
				this->inGoal = false;
			}
		}
	}

	//�ړ��ʐݒ�
	void Object::AddForce(ML::Vec2 dis_, float speed_, Direction dir_) {
		this->isMove = true;
		this->est = dis_;
		this->moveSpeed = speed_;
		this->dir = dir_;
	}

	void Object::Move() {
		if (abs(this->est.x) < 0.1f && abs(this->est.y) < 0.1f) {
			this->isMove = false;
			this->est = ML::Vec2(0, 0);
			return;
		}

		this->isMove = true;

		switch (this->dir) {
		case Direction::LEFT:
			if (abs(this->est.x) < this->moveSpeed) {
				this->pos.x += this->est.x;
				this->est.x = 0;
			}
			else {
				this->pos.x -= this->moveSpeed;
				this->est.x += this->moveSpeed;
			}
			break;
		case Direction::RIGHT:
			if (abs(this->est.x) < this->moveSpeed) {
				this->pos.x += this->est.x;
				this->est.x = 0;
			}
			else {
				this->pos.x += this->moveSpeed;
				this->est.x -= this->moveSpeed;
			}
			break;
		case Direction::UP:
			if (abs(this->est.y) < this->moveSpeed) {
				this->pos.y += this->est.y;
				this->est.y = 0;
			}
			else {
				this->pos.y -= this->moveSpeed;
				this->est.y += this->moveSpeed;
			}
			break;
		case Direction::DOWN:
			if (abs(this->est.y) < this->moveSpeed) {
				this->pos.y += this->est.y;
				this->est.y = 0;
			}
			else {
				this->pos.y += this->moveSpeed;
				this->est.y -= this->moveSpeed;
			}
			break;
		}
		this->canTeleport = true;
	}

	bool Object::IsNear(ML::Vec2 pos_) {
		if (this->CompareVec2(pos_) && !this->inWater && !this->isMove) return true;
		return false;
	}

	bool Object::IsPushable(ML::Vec2 next_) {
		for (auto it = ge->qa_Boxs->begin(); it != ge->qa_Boxs->end(); ++it) {
			if ((*it)->IsNear(next_)) return false;
		}

		if (ge->qa_Map->CheckHit(next_, Box::defName)) {
			this->est = ML::Vec2(0, 0);
			return false;
		}
		return true;
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}