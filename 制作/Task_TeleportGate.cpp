//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_TeleportGate.h"

namespace  TeleportGate
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgPort = DG::Image::Create("./data/image/effect/teleportGate.png");
		this->sdTeleport = DM::Sound::CreateSE("./data/audio/teleport.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgPort.reset();
		this->sdTeleport.reset();
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
		this->timeCnt++;

		if (this->isWork) {
			//�Ή��̃Q�C�g��T��
			for (auto it = ge->qa_TeleportGates->begin(); it != ge->qa_TeleportGates->end(); ++it) {
				if ((*it)->number == this->number) {
					if (!this->CompareVec2((*it)->pos)) {
						this->pairPos = (*it)->pos;
					}
				}
			}
			
			if (this->isBlocked()) return;

			//��ɂ���I�u�W�F�N�g��]������
			auto go = ge->GetTasks<GameObject>("GameObject");
			for (auto it = go->begin(); it != go->end(); ++it) {
				if (this->CompareVec2((*it)->pos)) {
					if ((*it)->canTeleport && !(*it)->isMove) {
						(*it)->pos = this->pairPos;
						(*it)->canTeleport = false;
						this->res->sdTeleport->Play_Normal(false);
					}
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (this->isWork) {
			ML::Box2D draw(-64, -88, 128, 128);
			draw.Offset(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			int animTable[] = { 0, 1, 2, 1 };
			int animFlame = animTable[this->timeCnt / 15 % 4];
			ML::Box2D src(animFlame * 64, this->number * 112, 64, 112);
			this->res->imgPort->Draw(draw, src);
		}
	}

	void Object::Open() {
		this->isWork = true;
	}
	void Object::Close() {
		this->isWork = false;
	}
	bool Object::isBlocked() {
		auto go = ge->GetTasks<GameObject>("GameObject");
		for (auto it = go->begin(); it != go->end(); ++it) {
			if ((*it)->CompareVec2(this->pairPos)) return true;
		}
		return false;
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