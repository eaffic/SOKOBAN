//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_Title.h"
#include  "Task_NewStage.h"
#include  "Task_MapObject.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgHint = DG::Image::Create("./data/image/text/hint.png");
		this->font = DG::Font::Create("MS �S�V�b�N", 16, 24);
		this->sdClear = DM::Sound::CreateSE("./data/audio/clear.wav");
		this->sdAllClear = DM::Sound::CreateSE("./data/audio/congraguation.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgHint.reset();
		this->font.reset();
		this->sdClear.reset();
		this->sdAllClear.reset();
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
		this->render2D_Priority[1] = 0.1f;
		this->startStep = ge->system.step;
		bgm::LoadFile("game", "./data/audio/bgm2.wav");
		bgm::Play("game");
		bgm::VolumeControl("game", 80);

		//���^�X�N�̐���
		ge->qa_Debug = DebugLog::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G(Map::defGroupName);
		ge->KillAll_G(MapObject::defGroupName);
		ge->KillAll_G("GameObject");
		ge->KillAll_G("EventItem");
		ge->KillAll_G(DebugLog::defGroupName);
		ge->KillAll_G(Key::defGroupName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			if (ge->system.restart) {
				auto nextTask = NewStage::Object::Create(true);
				nextTask->stage = this->stage;
				ge->system.step = this->startStep;
				ge->system.restart = false;
			}
			else if (this->stage == ge->system.maxStage) {
				Title::Object::Create(true);
			}
			else {
				auto nextTask = NewStage::Object::Create(true);
				nextTask->stage = this->stage + 1;
			}
			ge->CreateEffect(96, ML::Vec2(0, 0));
			bgm::Stop("game");
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();

		//debug
		if (inp.B3.down) {
			this->Kill();
		}

		switch (ge->system.gameState) {
		case MyPG::MyGameEngine::GAMESTATE::INIT:
			ge->system.timeCount = 30;
			ge->system.gameState = MyPG::MyGameEngine::GAMESTATE::WAIT;
			break;
		case MyPG::MyGameEngine::GAMESTATE::WAIT:
			ge->system.timeCount--;
			if (ge->system.timeCount <= 0) {
				ge->system.gameState = MyPG::MyGameEngine::GAMESTATE::PLAY;
			}
			break;
		case MyPG::MyGameEngine::GAMESTATE::START:
			break;
		case MyPG::MyGameEngine::GAMESTATE::PLAY:
			//�f�o�b�O���[�h
			if (inp.Trigger.L2.down) {
				ge->system.debug = !ge->system.debug;
			}
			//�J�����ړ����[�h
			if (inp.B1.on) {
				ge->system.moveCamera = true;
			}
			else {
				ge->system.moveCamera = false;
			}
			//�X�e�[�W���Z�b�g
			if (inp.Trigger.R2.down) {
				ge->system.timeCount = 40;
				ge->CreateEffect(97, ML::Vec2(0, 0));
				ge->system.gameState = MyPG::MyGameEngine::GAMESTATE::CHANGESTAGE;
				ge->system.restart = true;
			}
			break;
		case MyPG::MyGameEngine::GAMESTATE::STOP:
			ge->system.timeCount = 300;
			ge->CreateEffect(5, ML::Vec2(0, 0));
			this->res->sdClear->Play_Normal(false);
			bgm::Stop("game");
			ge->system.gameState = MyPG::MyGameEngine::GAMESTATE::CLEAR;
			break;
		case MyPG::MyGameEngine::GAMESTATE::CLEAR:
			ge->system.timeCount--;
			if (ge->system.timeCount < 0) {
				//�S�X�e�[�W�N���A����
				if (this->stage == ge->system.maxStage) {
					ge->system.timeCount = 360;
					ge->CreateEffect(6, ML::Vec2(0, 0));
					ge->StartCounter("cleareffect", 10);
					this->res->sdAllClear->Play_Normal(false);
					ge->system.gameState = MyPG::MyGameEngine::GAMESTATE::END;
				}
				else {
					ge->system.timeCount = 40;
					ge->CreateEffect(97, ML::Vec2(0, 0));
					ge->system.gameState = MyPG::MyGameEngine::GAMESTATE::CHANGESTAGE;
				}
			}
			break;
		case MyPG::MyGameEngine::GAMESTATE::CHANGESTAGE:
			ge->system.timeCount--;
			if (ge->system.timeCount <= 0) {
				this->Kill();
			}
			break;
		case MyPG::MyGameEngine::GAMESTATE::END:
			ge->system.timeCount--;

			//�N���A�G�t�F�N�g
			if (ge->getCounterFlag("cleareffect") == ge->END) {
				ge->StartCounter("cleareffect", 15);
				ge->CreateEffect(2, ML::Vec2(0, 0));
				ge->CreateEffect(2, ML::Vec2(0, 0));
				ge->CreateEffect(2, ML::Vec2(0, 0));
				ge->CreateEffect(2, ML::Vec2(0, 0));
				ge->CreateEffect(2, ML::Vec2(0, 0));
			}

			if (ge->system.timeCount <= 0) {
				ge->system.timeCount = 40;
				ge->CreateEffect(97, ML::Vec2(0, 0));
				ge->system.gameState = MyPG::MyGameEngine::GAMESTATE::CHANGESTAGE;
			}
			break;
		}

		bgm::EndCheck();
		ge->ActCounter();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(420, 580, 830, 110);
		ML::Box2D src(0, 0, 480, 59);
		this->res->imgHint->Draw(draw, src);

		draw = ML::Box2D(50, 50, 200, 200);
		if(ge->system.gameState == MyPG::MyGameEngine::GAMESTATE::PLAY)
			this->res->font->Draw(draw, "Step: " + to_string(ge->system.step), ML::Color(1, 1, 1, 0));
		if(ge->system.gameState == MyPG::MyGameEngine::GAMESTATE::END)
			if (this->stage == ge->system.maxStage) {
				draw = ML::Box2D(500, 380, 400, 100);
				this->res->font->Draw(draw, "Total Step: " + to_string(ge->system.step), ML::Color(1, 1, 0, 0));
			}

		draw = ML::Box2D(1000, 50, 300, 200);
		this->res->font->Draw(draw, "Stage: " + to_string(this->stage));
	}

	//-------------------------------------------------------------------
	void Object::MapSet(int stage_) {
		this->stage = stage_;
		ge->qa_Map = Map::Object::Create(true);
		ge->qa_Map->CreateMap(this->stage);
		ge->qa_MapObject = MapObject::Object::Create(true);
		ge->qa_MapObject->ObjectSetting(this->stage);
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