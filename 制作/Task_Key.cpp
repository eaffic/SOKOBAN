//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Key.h"

namespace  Key
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgPressurePlate = DG::Image::Create("./data/image/map/Pressure Plate.png");
		this->sdPressed = DM::Sound::CreateSE("./data/audio/button.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgPressurePlate.reset();
		this->sdPressed.reset();
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
		this->render2D_Priority[1] = 0.7f;
		this->pos = ML::Vec2(PIXELSIZE / 2, PIXELSIZE / 2);
		this->number = 0;
		this->isPressed = false;

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
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(-PIXELSIZE / 2, -PIXELSIZE / 2, PIXELSIZE, PIXELSIZE);
		draw.Offset(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		ML::Box2D src(0, 0, 32, 32);
		this->res->imgPressurePlate->Draw(draw, src);
	}

	//キーの上に物体があるか？
	bool Object::CheckBlock() {
		auto go = ge->GetTasks<GameObject>("GameObject");
		for (auto it = go->begin(); it != go->end(); ++it) {
			if (this->CompareVec2((*it)->pos)) return true;
		}
		return false;
	}

	//対応の目標を起動する
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

	//対応の目標を閉じる
	void Object::CloseTarget() {
		if (!this->isPressed) return;

		auto items = ge->GetTasks<EventItem>("EventItem");
		for (auto it = items->begin(); it != items->end(); ++it) {
			if ((*it)->number == this->number) {
				(*it)->Close();
			}
		}
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