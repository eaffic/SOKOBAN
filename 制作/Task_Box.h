#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameObject.h"

namespace  Box
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("GameObject");	//グループ名
	const  string  defName("Box");	//タスク名
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
		DG::Image::SP	imgBox;
		DM::Sound::SP	sdMove;
		DM::Sound::SP	sdWater;
	};
	//-------------------------------------------------------------------
	class  Object : public  GameObject
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
		//追加したい変数・メソッドはここに追加する
		int moveSpeed; //移動速度

		ML::Vec2 est; //予定移動量

		int stopTime;
		int number;
		bool inWater;
		bool inGoal;
		bool seplay;

		void RenderSet();
		void AddForce(ML::Vec2 dis_, float speed_, Direction dir_); //移動量設定
		void Move(); //移動
		bool IsNear(ML::Vec2 pos_); //プレイヤー確認
		bool IsPushable(ML::Vec2 next_); //移動先確認
	};
}