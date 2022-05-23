//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_TeleportGate.h"

namespace  TeleportGate
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgPort = DG::Image::Create("./data/image/effect/teleportGate.png");
		this->sdTeleport = DM::Sound::CreateSE("./data/audio/teleport.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgPort.reset();
		this->sdTeleport.reset();
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
		this->timeCnt++;

		if (this->isWork) {
			//対応のゲイトを探す
			for (auto it = ge->qa_TeleportGates->begin(); it != ge->qa_TeleportGates->end(); ++it) {
				if ((*it)->number == this->number) {
					if (!this->CompareVec2((*it)->pos)) {
						this->pairPos = (*it)->pos;
					}
				}
			}
			
			if (this->isBlocked()) return;

			//上にあるオブジェクトを転送する
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
	//「２Ｄ描画」１フレーム毎に行う処理
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