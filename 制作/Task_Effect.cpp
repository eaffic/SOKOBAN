//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect.h"

namespace  Effect
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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
		this->render2D_Priority[1] = 0.f;

		//�ړ���
		this->pos = ML::Vec2(0, 0);
		this->moveVec = ML::Vec2(0, 0);
		this->gravityVec = ML::Vec2(0, 0);

		//�p�x
		this->angle = 0.f;
		this->addAngle = 0.f;
		this->fadeMaxAngle = 180.0f;

		//�g�嗦
		this->scale = 1.f;
		this->addScale = 0.f;
		this->maxScale = -1.f;

		//�����x
		this->addRender = false;
		this->alpha = 1.f;
		this->addAlpha = 0.f;

		//���\�[�X�T�C�Y
		this->resSizeX = 256;
		this->resSizeY = 256;
		this->drawSizeX = 64;
		this->drawSizeY = 64;

		//�A�j���[�V����
		this->animno = 0;
		this->addAnim = 0.f;
		this->animMax = 0;
		this->animResNum = 1;

		//�J�E���g
		this->count = 0;
		this->countMax = 30;

		this->filename = "";

		this->fade = false;
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
		if (this->fade) {
			this->alpha = sin(ML::ToRadian(this->angle));
			this->angle += this->addAngle;
			if (this->angle >= this->fadeMaxAngle) {
				this->Kill();
			}
		}
		else {
			this->pos += this->moveVec;
			this->moveVec += this->gravityVec;

			this->angle += this->addAngle;
			this->alpha += this->addAlpha;
			this->scale += this->addScale;

			if (this->maxScale > 0)
				if (this->scale > this->maxScale)
					this->scale = this->maxScale;

			this->count++;
			if (this->animMax == 0) {
				if (this->count >= this->countMax) {
					this->Kill();
				}
			}
			else
			{
				this->animno += this->addAnim;

				if ((int)this->animno >= this->animMax) {
					this->Kill();
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (this->count == 0) {
			if (this->filename.empty()) this->Kill();
			this->img = DG::Image::Create(filename);
		}

		ML::Box2D draw((int)(-this->drawSizeX / 2 * this->scale), (int)(-this->drawSizeY / 2 * this->scale), (int)(this->drawSizeX * this->scale), (int)(this->drawSizeY * this->scale));
		draw.Offset(this->pos);

		if (this->animResNum <= 0)this->animResNum = 1;
		ML::Box2D src((int)(this->animno % this->animResNum) * this->resSizeX, (int)(this->animno / this->animResNum) * this->resSizeY, this->resSizeX, this->resSizeY);

		if (fade != true) {
			this->img->Rotation(this->angle, ML::Vec2(draw.w / 2.0f, draw.h / 2.0f));
		}

		if (this->addRender) {
			ge->dgi->Flush2D();
			ge->dgi->EffectState().BS_AlphaAdd();
		}

		this->img->Draw(draw, src, ML::Color(this->alpha, 1, 1, 1));

		this->img->Rotation(0, ML::Vec2(draw.w / 2.0f, draw.h / 2.0f));

		if (this->addRender) {
			ge->dgi->Flush2D();
			ge->dgi->EffectState().BS_Alpha();
		}
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