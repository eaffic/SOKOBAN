//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Door.h"

namespace  Door
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgDoor = DG::Image::Create("./data/image/map/TX Props.png");
		this->sdOpen = DM::Sound::CreateSE("./data/audio/dooropen.wav");
		this->sdClose = DM::Sound::CreateSE("./data/audio/doorclose.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgDoor.reset();
		this->sdOpen.reset();
		this->sdClose.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->render2D_Priority[1] = 0.3f;
		this->pos = ML::Vec2(PIXELSIZE / 2, PIXELSIZE / 2);
		this->number = 0;
		this->isWork = false;

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(-96, -96, 192, 128);
		draw.Offset(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		if (this->isWork) {
			ML::Box2D src(0, 160, 96, 64);
			this->res->imgDoor->Draw(draw, src);
		}
		else {
			ML::Box2D src(0, 96, 96, 64);
			this->res->imgDoor->Draw(draw, src);
		}
	}

	void Object::Open() {
		this->isWork = true;
		ge->qa_Map->arrMap[ge->qa_Map->BARRIER][int(this->pos.y) / PIXELSIZE][int(this->pos.x) / PIXELSIZE] = -1;
		this->res->sdOpen->Play_Normal(false);
		this->res->sdOpen->SetVolume(30);
	}


	void Object::Close() {
		this->isWork = false;
		ge->qa_Map->arrMap[ge->qa_Map->BARRIER][int(this->pos.y) / PIXELSIZE][int(this->pos.x) / PIXELSIZE] = 1;
		this->res->sdClose->Play_Normal(false);
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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