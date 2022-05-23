//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_NewStage.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgTitle = DG::Image::Create("./data/image/titleimage.png");
		this->imgText = DG::Image::Create("./data/image/text/titlefont.png");
		this->imgStart = DG::Image::Create("./data/image/text/titlestart.png");
		this->sdStart = DM::Sound::CreateSE("./data/audio/start.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgTitle.reset();
		this->imgText.reset();
		this->imgStart.reset();
		this->sdStart.reset();
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
		ge->camera2D = ML::Box2D(0, 0, 1280, 720);

		this->render2D_Priority[1] = 0.5f;
		this->startTime = 0;
		this->src = ML::Box2D(50, 150, 1400, 700);

		this->imgTitleAlpha = 0;
		this->imgStartAlpha = 0;
		this->delta = 0;

		bgm::LoadFile("title", "./data/audio/bgm1.mp3");
		bgm::Play("title");
		bgm::VolumeControl("title", 80);

		ge->system.maxStage = 8;
		ge->system.step = 0;
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
			auto nextTask = NewStage::Object::Create(true);
			nextTask->stage = 1;
			ge->CreateEffect(98, ML::Vec2(0, 0));
			bgm::Stop("title");
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();
		auto mouse = ge->mouse->GetState();

		if (this->startTime >= 100 && (inp.ST.down || mouse.LB.down)) {
			ge->StartCounter("title", 40);
			ge->CreateEffect(99, ML::Vec2(0, 0));
			this->res->sdStart->Play_Normal(false);
		}

		if (ge->getCounterFlag("title") == ge->END) {
			ge->InitCounter();
			this->Kill();
		}

		bgm::EndCheck();

		ge->ActCounter();

		//�_�Ō���
		if (this->imgStartAlpha > 0.8) { this->delta = -0.02; }
		else if (this->imgStartAlpha < 0.2) { this->delta = 0.02; }
		
		this->startTime++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (this->startTime >= 30) {
			ML::Box2D draw(0, 0, 1280, 720);
			this->src.y > 20 ? this->src.y-=2 : this->src.y;
			this->res->imgTitle->Draw(draw, this->src);
		}

		if (this->src.y <= 20) {
			ML::Box2D draw(440, 50, 400, 100);
			ML::Box2D src(0, 0, 441, 84);
			this->res->imgText->Draw(draw, src, ML::Color(this->imgTitleAlpha < 1 ? this->imgTitleAlpha += 0.03f : 1, 1, 1, 1));

			draw = ML::Box2D(350, 450, 580, 100);
			src = ML::Box2D(0, 0, 697, 96);
			this->res->imgStart->Draw(draw, src, ML::Color(this->imgStartAlpha += this->delta, 1, 1, 1));
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