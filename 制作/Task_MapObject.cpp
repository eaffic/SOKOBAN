//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapObject.h"
#include  "Task_Door.h"

namespace  MapObject
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgGoal = DG::Image::Create("./data/image/effect/goal.png");
		this->imgMesh = DG::Image::Create("./data/map/mesh.png");
		this->imgBoxGoal = DG::Image::Create("./data/image/map/boxgoal.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgGoal.reset();
		this->imgMesh.reset();
		this->imgBoxGoal.reset();
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
		this->render2D_Priority[1] = 0.6f;
		this->stage = 0;
		this->goalNum = 0;
		this->timeCnt = 0;
		this->reStart = false;
		this->boxClear = false;

		for (int z = 0; z < LAYERMAX; ++z) {
			for (int y = 0; y < MAPY; ++y) {
				for (int x = 0; x < MAPX; ++x) {
					this->arrObject[z][y][x] = -1;
				}
			}
		}

		for (int i = 0; i < 20; ++i) {
			this->goalBoxPos[i] = ML::Vec2(-1, -1);
		}
		this->goalPlayerPos = ML::Vec2(-1, -1);

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
		auto inp = ge->in1->GetState();

		//デバッグモード
		ge->qa_Debug->DebugPrint("Box Number:" + to_string(ge->qa_Boxs->size()));
		ge->qa_Debug->DebugPrint("Now Stage: " + to_string(this->stage));

		//プレイ中だけクリア判定を行う
		if (ge->system.gameState == MyPG::MyGameEngine::GAMESTATE::PLAY) {
			this->CheckClearBox();
			this->CheckClearPlayer();
		}

		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, ge->screen2DWidth, ge->screen2DHeight);
		ML::Box2D src(0, 0, 640, 360);
		//this->res->imgMesh->Draw(draw, src);
		int animTable[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		int animResNum = 5;

		for (int i = 0; i < 20; ++i) {
			if (this->goalBoxPos[i].x != -1) {
				draw = ML::Box2D(-PIXELSIZE / 2, -PIXELSIZE / 2, PIXELSIZE, PIXELSIZE);
				src = ML::Box2D(0, 0, 32, 32);
				draw.Offset(this->goalBoxPos[i] + ML::Vec2(PIXELSIZE / 2, PIXELSIZE / 2));
				draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
				this->res->imgBoxGoal->Draw(draw, src, ML::Color(0.7f, 1, 1, 1));
			}
		}

		if(this->boxClear) {
			draw = ML::Box2D(-64, -72, 128, 128);
			draw.Offset(this->goalPlayerPos + ML::Vec2(PIXELSIZE / 2, PIXELSIZE / 2));
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			src = ML::Box2D((int)(animTable[this->timeCnt / 5 % 10] % animResNum) * 192, (int)(animTable[this->timeCnt / 5 % 10] / animResNum) * 192, 192, 192);
			this->res->imgGoal->Draw(draw, src);
		}
	}

	//-------------------------------------------------------------------
	//箱クリア判定
	void Object::CheckClearBox() {
		if (this->goalNum == 0) {
			this->boxClear = true;
			return;
		}

		int goalCount = 0;
		for (auto it = ge->qa_Boxs->begin(); it != ge->qa_Boxs->end(); ++it) {
			for (int i = 0; i < 20; ++i) {
				if (this->goalBoxPos[i].x == -1 || (*it)->isMove) break;
				if ((int)(*it)->pos.x / PIXELSIZE == (int)this->goalBoxPos[i].x / PIXELSIZE && (int)(*it)->pos.y / PIXELSIZE == (int)this->goalBoxPos[i].y / PIXELSIZE) goalCount++;
			}
		}
		if (goalCount == this->goalNum) { this->boxClear = true; }
		else { this->boxClear = false; }
	}

	//クリア判定
	void Object::CheckClearPlayer() {
		if (!this->boxClear) return;

		auto player = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
		if (player->CompareVec2(this->goalPlayerPos)) { ge->system.gameState = MyPG::MyGameEngine::GAMESTATE::STOP; }
	}

	//功能性オブジェクト読み込み、設置
	void Object::ObjectSetting(int stage_) {
		this->stage = stage_;

		this->Load();

		int boxNum = 0;
		for (int z = 0; z < LAYERMAX; ++z) {
			for (int y = 0; y < MAPY; ++y) {
				for (int x = 0; x < MAPX; ++x) {
					int no = this->arrObject[z][y][x];
					if (no == -1) continue;

					switch (z) {
					case OBJECTLAYER::PLAYER:
					{
						auto player = Player::Object::Create(true);
						player->pos += ML::Vec2(PIXELSIZE * x, PIXELSIZE * y);
						break;
					}
					case OBJECTLAYER::BOX:
					{
						auto box = Box::Object::Create(true);
						box->pos += ML::Vec2(PIXELSIZE * x, PIXELSIZE * y);
						box->number = boxNum;
						box->render2D_Priority[1] += boxNum * 0.005f;
						boxNum++;
						break;
					}
					case OBJECTLAYER::GOAL:
						if (no == 4) {
							for (int i = 0; i < 20; ++i) {
								if (this->goalBoxPos[i].x == -1) {
									this->goalBoxPos[i] = ML::Vec2(PIXELSIZE * x, PIXELSIZE * y);
									this->goalNum++;
									break;
								}
							}
						}
						else {
							this->goalPlayerPos = ML::Vec2(PIXELSIZE * x, PIXELSIZE * y);
						}
						break;
					case OBJECTLAYER::TELEPORTGATE:
					{
						auto teleportGate = TeleportGate::Object::Create(true);
						teleportGate->pos += ML::Vec2(PIXELSIZE * x, PIXELSIZE * y);
						teleportGate->number = this->arrObject[z][y][x];
						break;
					}
					case OBJECTLAYER::KEY:
					{
						auto key = Key::Object::Create(true);
						key->pos += ML::Vec2(PIXELSIZE * x, PIXELSIZE * y);
						key->number = this->arrObject[z][y][x];
						break;
					}
					case OBJECTLAYER::DOOR:
					{
						auto door = Door::Object::Create(true);
						door->pos += ML::Vec2(PIXELSIZE * x, PIXELSIZE * y);
						door->number = this->arrObject[z][y][x];
						break;
					}
					}
				}
			}
		}

		auto map = Map::Object::Create(true);
		map->CreateMap(this->stage);

		//マップ上のオブジェクトを登録する
		ge->qa_Boxs = ge->GetTasks<Box::Object>(Box::defGroupName, Box::defName);
		ge->qa_Map = ge->GetTask<Map::Object>(Map::defGroupName, Map::defName);
		ge->qa_TeleportGates = ge->GetTasks<TeleportGate::Object>(TeleportGate::defGroupName, TeleportGate::defName);

	}

	//ファイル読み込み
	bool Object::Load() {
		string filePath[LAYERMAX]{
			"./data/map/stage/" + to_string(this->stage) + "/Stage" + to_string(this->stage) + "_Player.csv",
			"./data/map/stage/" + to_string(this->stage) + "/Stage" + to_string(this->stage) + "_Box.csv",
			"./data/map/stage/" + to_string(this->stage) + "/Stage" + to_string(this->stage) + "_Goal.csv",
			"./data/map/stage/" + to_string(this->stage) + "/Stage" + to_string(this->stage) + "_TeleportGate.csv",
			"./data/map/stage/" + to_string(this->stage) + "/Stage" + to_string(this->stage) + "_Key.csv",
			"./data/map/stage/" + to_string(this->stage) + "/Stage" + to_string(this->stage) + "_Door.csv"
		};

		for (int file = 0; file < LAYERMAX; ++file) {
			ifstream fin(filePath[file]);
			if (!fin) continue;

			string str;
			int y = 0;
			while (getline(fin, str)) {
				string::size_type t;
				while ((t = str.find_first_of(',')) != string::npos) {
					str[t] = ' ';
				}
				istringstream stream(str);
				for (int x = 0; x < MAPX; ++x) {
					stream >> this->arrObject[file][y][x];
				}
				y++;
			}
			fin.close();
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