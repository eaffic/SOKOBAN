//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map.h"


namespace  Map
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgGrass = DG::Image::Create("./data/image/map/TX Tileset Grass.png");
		this->imgPlant = DG::Image::Create("./data/image/map/TX Plant.png");
		this->imgPlantShadow = DG::Image::Create("./data/image/map/TX Shadow Plant.png");
		this->imgProps = DG::Image::Create("./data/image/map/TX Props.png");
		this->imgShadow = DG::Image::Create("./data/image/map/TX Shadow.png");
		this->imgStoneGround = DG::Image::Create("./data/image/map/TX Tileset Stone Ground.png");
		this->imgStruct = DG::Image::Create("./data/image/map/TX Struct.png");
		this->imgWall = DG::Image::Create("./data/image/map/TX Tileset Wall.png");
		this->imgWater = DG::Image::Create("./data/image/map/[A]Water2_pipo.png");

		this->imgBarrier = DG::Image::Create("./data/map/EventFlag.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgGrass.reset();
		this->imgPlant.reset();
		this->imgPlantShadow.reset();
		this->imgProps.reset();
		this->imgShadow.reset();
		this->imgStoneGround.reset();
		this->imgStruct.reset();
		this->imgWall.reset();

		this->imgBarrier.reset();
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
		this->render2D_Priority[1] = 0.9f;
		this->hitBase = ML::Box2D(0, 0, PIXELSIZE * MAPX, PIXELSIZE * MAPY);

		for (int z = 0; z < LAYERMAX; ++z) {
			for (int y = 0; y < MAPY; ++y) {
				for (int x = 0; x < MAPX; ++x) {
					this->arrMap[z][y][x] = -1;
				}
			}
		}

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
		if (!this->hitBase.Hit(ge->camera2D)) return;

		ML::Rect camera = {
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h,
		};
		ML::Rect map = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h,
		};

		int sx, sy, ex, ey;
		sx = max(camera.left, map.left) / PIXELSIZE;
		sy = max(camera.top, map.top) / PIXELSIZE;
		ex = max(camera.right, map.right) / PIXELSIZE;
		ey = max(camera.bottom, map.bottom) / PIXELSIZE;

		for (int z = 0; z < LAYERMAX; ++z) {
			for (int y = sy; y < ey; ++y) {
				for (int x = sx; x < ex; ++x) {
					int no = this->arrMap[z][y][x];
					if (no == -1) continue;

					ML::Box2D draw(0, 0, PIXELSIZE, PIXELSIZE);
					draw.Offset(PIXELSIZE * x, PIXELSIZE * y);
					draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

					int dx, dy;
					ML::Box2D src(0, 0, 32, 32);
					switch (z) {
					case MAPLAYER::GRASS:
						dx = no % 8 * 32;
						dy = no / 8 * 32;
						src = ML::Box2D(dx, dy, 32, 32);
						this->res->imgGrass->Draw(draw, src);
						break;
					case MAPLAYER::STONEGROUND:
						dx = no % 8 * 32;
						dy = no / 8 * 32;
						src = ML::Box2D(dx, dy, 32, 32);
						this->res->imgStoneGround->Draw(draw, src);
						break;
					case MAPLAYER::WATER:
						dx = no % 8 * 32;
						dy = no / 8 * 32;
						src = ML::Box2D(dx, dy, 32, 32);
						this->res->imgWater->Draw(draw, src);
					case MAPLAYER::PLANTSHADOW:
						dx = no % 16 * 32;
						dy = no / 16 * 32;
						src = ML::Box2D(dx, dy, 32, 32);
						this->res->imgPlantShadow->Draw(draw, src);
						break;
					case MAPLAYER::SHADOW:
						dx = no % 16 * 32;
						dy = no / 16 * 32;
						src = ML::Box2D(dx, dy, 32, 32);
						this->res->imgShadow->Draw(draw, src);
						break;
					case MAPLAYER::WALL:
						dx = no % 16 * 32;
						dy = no / 16 * 32;
						src = ML::Box2D(dx, dy, 32, 32);
						this->res->imgWall->Draw(draw, src);
						break;
					case MAPLAYER::STRUCT:
						dx = no % 16 * 32;
						dy = no / 16 * 32;
						src = ML::Box2D(dx, dy, 32, 32);
						this->res->imgStruct->Draw(draw, src);
						break;
					case MAPLAYER::PLANT:
						dx = no % 16 * 32;
						dy = no / 16 * 32;
						src = ML::Box2D(dx, dy, 32, 32);
						this->res->imgPlant->Draw(draw, src);
						break;
					case MAPLAYER::PROPS:
						dx = no % 16 * 32;
						dy = no / 16 * 32;
						src = ML::Box2D(dx, dy, 32, 32);
						this->res->imgProps->Draw(draw, src);
						break;
					case MAPLAYER::BARRIER:
						if(ge->system.debug)
							this->res->imgBarrier->Draw(draw, ML::Box2D(288, 0, 32, 32), ML::Color(0.5, 0, 0, 0));
						break;
					case MAPLAYER::WATERBARRIER:
						if(ge->system.debug)
							this->res->imgBarrier->Draw(draw, ML::Box2D(256, 0, 32, 32));
						break;
					}
				}
			}
		}
	}

	//-------------------------------------------------------------------
	void Object::AdjustCameraPos() {
		ML::Rect camera = {
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h,
		};
		ML::Rect map = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h,
		};

		if (camera.right > map.right) { ge->camera2D.x = map.right - ge->camera2D.w; }
		if (camera.bottom > map.bottom) { ge->camera2D.y = map.bottom - ge->camera2D.h; }
		if (camera.left < map.left) { ge->camera2D.x = map.left; }
		if (camera.top < map.top) { ge->camera2D.y = map.top; }

		if (this->hitBase.w < ge->camera2D.w) { ge->camera2D.x = map.left; }
		if (this->hitBase.h < ge->camera2D.h) { ge->camera2D.y = map.top; }
	}

	void Object::CreateMap(int stage_) {
		string filePath[LAYERMAX] = {
			"./data/map/stage/" + to_string(stage_) + "/Stage" + to_string(stage_) + "_Grass.csv",
			"./data/map/stage/" + to_string(stage_) + "/Stage" + to_string(stage_) + "_StoneGround.csv",
			"./data/map/stage/" + to_string(stage_) + "/Stage" + to_string(stage_) + "_Water.csv",
			"./data/map/stage/" + to_string(stage_) + "/Stage" + to_string(stage_) + "_PlantShadow.csv",
			"./data/map/stage/" + to_string(stage_) + "/Stage" + to_string(stage_) + "_Shadow.csv",
			"./data/map/stage/" + to_string(stage_) + "/Stage" + to_string(stage_) + "_Wall.csv",
			"./data/map/stage/" + to_string(stage_) + "/Stage" + to_string(stage_) + "_Struct.csv",
			"./data/map/stage/" + to_string(stage_) + "/Stage" + to_string(stage_) + "_Plant.csv",
			"./data/map/stage/" + to_string(stage_) + "/Stage" + to_string(stage_) + "_Props.csv",
			"./data/map/stage/" + to_string(stage_) + "/Stage" + to_string(stage_) + "_Barrier.csv",
			"./data/map/stage/" + to_string(stage_) + "/Stage" + to_string(stage_) + "_WaterBarrier.csv",
		};

		for (int file = 0; file < LAYERMAX; ++file) {
			ifstream fin(filePath[file]);
			if (!fin) continue;

			string str;
			int y = 0;
			while (getline(fin, str)) {
				string::size_type t;
				//カンマをスペースに書き換え
				while ((t = str.find_first_of(',')) != string::npos) {
					str[t] = ' ';
				}
				istringstream stream(str);
				for (int x = 0; x < MAPX; ++x) {
					stream >> this->arrMap[file][y][x];
				}
				y++;
			}
			fin.close();
		}
	}
	
	bool Object::CheckHit(ML::Vec2 pos_, string tag_) {
		int x = pos_.x / PIXELSIZE;
		int y = pos_.y / PIXELSIZE;
		if (tag_ == Box::defName) {
			if (this->arrMap[MAPLAYER::STRUCT][y][x] >= 0) return true;
		}
		if (tag_ == Player::defName) {
			if (ge->system.debug) return false;
			if (this->arrMap[MAPLAYER::WATERBARRIER][y][x] >= 0) return true;
		}
		if (this->arrMap[MAPLAYER::BARRIER][y][x] >= 0) return true;
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