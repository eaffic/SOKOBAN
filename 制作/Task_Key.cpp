//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Key.h"

namespace  Key
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgPressurePlate = DG::Image::Create("./data/image/map/Pressure Plate.png");
		this->sdPressed = DM::Sound::CreateSE("./data/audio/button.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgPressurePlate.reset();
		this->sdPressed.reset();
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
		this->render2D_Priority[1] = 0.7f;
		this->pos = ML::Vec2(PIXELSIZE / 2, PIXELSIZE / 2);
		this->number = 0;
		this->isPressed = false;

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
		if (this->CheckBlock()) {
			this->OpenTarget();
			this->isPressed = true;
		}
		else {
			this->CloseTarget();
			this->isPressed = false;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(-PIXELSIZE / 2, -PIXELSIZE / 2, PIXELSIZE, PIXELSIZE);
		draw.Offset(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		ML::Box2D src(0, 0, 32, 32);
		this->res->imgPressurePlate->Draw(draw, src);
	}

	//�L�[�̏�ɕ��̂����邩�H
	bool Object::CheckBlock() {
		auto go = ge->GetTasks<GameObject>("GameObject");
		for (auto it = go->begin(); it != go->end(); ++it) {
			if (this->CompareVec2((*it)->pos)) return true;
		}
		return false;
	}

	//�Ή��̖ڕW���N������
	void Object::OpenTarget() {
		if (this->isPressed) return;
		this->res->sdPressed->Play_Normal(false);

		auto items = ge->GetTasks<EventItem>("EventItem");
		for (auto it = items->begin(); it != items->end(); ++it) {
			if ((*it)->number == this->number) {
				(*it)->Open();
			}
		}
	}

	//�Ή��̖ڕW�����
	void Object::CloseTarget() {
		if (!this->isPressed) return;

		auto items = ge->GetTasks<EventItem>("EventItem");
		for (auto it = items->begin(); it != items->end(); ++it) {
			if ((*it)->number == this->number) {
				(*it)->Close();
			}
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