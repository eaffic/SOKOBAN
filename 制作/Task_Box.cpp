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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgBox = DG::Image::Create("./data/image/map/TX Props.png");
		this->sdMove = DM::Sound::CreateSE("./data/audio/boxmove.wav");
		this->sdWater = DM::Sound::CreateSE("./data/audio/boxwater.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgBox.reset();
		this->sdMove.reset();
		this->sdWater.reset();
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
		this->RenderSet();

		//移動中音声設定
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
	//「２Ｄ描画」１フレーム毎に行う処理
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
	//描画レイヤー調整
	void Object::RenderSet() {
		if (this->inWater) return;

		//上下は他の箱がいるの場合
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

		//水に落ちた場合
		if (!this->isMove) {
			if (ge->qa_Map->arrMap[ge->qa_Map->WATERBARRIER][(int)this->pos.y / PIXELSIZE][(int)this->pos.x / PIXELSIZE] >= 0) {
				ge->qa_Map->arrMap[ge->qa_Map->WATERBARRIER][(int)this->pos.y / PIXELSIZE][(int)this->pos.x / PIXELSIZE] = -1;
				this->inWater = true;
				this->render2D_Priority[1] += 0.2f;
				this->AddForce(ML::Vec2(0, 36), 6, Direction::DOWN);
				this->res->sdWater->Play_Normal(false);
			}
		}

		//ゴールにいる場合
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

	//移動量設定
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