#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  MapObject
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("MapObject");	//グループ名
	const  string  defName("NoName");	//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		DG::Image::SP imgGoal;
		DG::Image::SP	imgMesh;
		DG::Image::SP	imgBoxGoal;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		#define MAPX (50)
		#define MAPY (50)

		enum OBJECTLAYER { PLAYER, BOX, GOAL, TELEPORTGATE, KEY, DOOR, LAYERMAX };

		//追加したい変数・メソッドはここに追加する
		int goalNum; //終点数
		int	stage; //ステージ番号
		int timeCnt;

		bool reStart; //ステージリセット
		bool boxClear;

		int arrObject[LAYERMAX][MAPY][MAPX];

		//目標位置
		ML::Vec2 goalBoxPos[20];
		ML::Vec2 goalPlayerPos;

		void ObjectSetting(int stage_); //ステージ初期化
		bool Load(); //csvファイル読み込み
		void CheckClearBox(); //クリア判定
		void CheckClearPlayer();
		void ResetGame();
	};
}