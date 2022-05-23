#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Map
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("Map");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
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
		//���L����ϐ��͂����ɒǉ�����
		DG::Image::SP	imgPlant;
		DG::Image::SP	imgPlantShadow;
		DG::Image::SP	imgProps;
		DG::Image::SP	imgShadow;
		DG::Image::SP	imgStruct;
		DG::Image::SP	imgGrass;
		DG::Image::SP	imgStoneGround;
		DG::Image::SP	imgWall;
		DG::Image::SP   imgWater;

		DG::Image::SP	imgBarrier;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		#define MAPX (50)
		#define MAPY (50)
		enum MAPLAYER { GRASS, STONEGROUND, WATER, PLANTSHADOW, SHADOW, WALL, STRUCT, PLANT, PROPS, BARRIER, WATERBARRIER, LAYERMAX };

		ML::Box2D hitBase;

		void AdjustCameraPos();
		void CreateMap(int stage_);
		bool CheckHit(ML::Vec2 pos_, string tag_);

		int arrMap[LAYERMAX][MAPY][MAPX];
	};
}