#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Effect
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("Effect");	//グループ名
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
		//追加したい変数・メソッドはここに追加する
		//リソース画像
		DG::Image::SP img;
		
		//移動量
		ML::Vec2 pos;					//位置
		ML::Vec2 moveVec;			//速度
		ML::Vec2 gravityVec;	//重力

		//角度
		float angle;
		float addAngle;				//角度変化
		float fadeMaxAngle;

		//拡大率
		float scale;
		float addScale;				//サイズ変化
		float maxScale;

		//透明度
		bool addRender; //加算合成
		float alpha;
		float addAlpha;				//透明度変化

		//リソースサイズ
		int resSizeX;
		int	resSizeY;
		int drawSizeX;
		int drawSizeY;

		//アニメーション
		int animno;						//アニメションコマ数
		int animMax;					//終了コマ数
		int	animResNum;				//リソースの行数
		float addAnim;				//アニメション加算

		//カウント
		int count;
		int countMax;				//最大維持時間

		string filename;


		bool fade;
	};
}