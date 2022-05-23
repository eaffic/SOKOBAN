#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  MapObject
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("MapObject");	//�O���[�v��
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
		DG::Image::SP imgGoal;
		DG::Image::SP	imgMesh;
		DG::Image::SP	imgBoxGoal;
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
		#define MAPX (50)
		#define MAPY (50)

		enum OBJECTLAYER { PLAYER, BOX, GOAL, TELEPORTGATE, KEY, DOOR, LAYERMAX };

		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		int goalNum; //�I�_��
		int	stage; //�X�e�[�W�ԍ�
		int timeCnt;

		bool reStart; //�X�e�[�W���Z�b�g
		bool boxClear;

		int arrObject[LAYERMAX][MAPY][MAPX];

		//�ڕW�ʒu
		ML::Vec2 goalBoxPos[20];
		ML::Vec2 goalPlayerPos;

		void ObjectSetting(int stage_); //�X�e�[�W������
		bool Load(); //csv�t�@�C���ǂݍ���
		void CheckClearBox(); //�N���A����
		void CheckClearPlayer();
		void ResetGame();
	};
}