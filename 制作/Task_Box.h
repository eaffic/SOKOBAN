#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameObject.h"

namespace  Box
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("GameObject");	//�O���[�v��
	const  string  defName("Box");	//�^�X�N��
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
		DG::Image::SP	imgBox;
		DM::Sound::SP	sdMove;
		DM::Sound::SP	sdWater;
	};
	//-------------------------------------------------------------------
	class  Object : public  GameObject
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
		int moveSpeed; //�ړ����x

		ML::Vec2 est; //�\��ړ���

		int stopTime;
		int number;
		bool inWater;
		bool inGoal;
		bool seplay;

		void RenderSet();
		void AddForce(ML::Vec2 dis_, float speed_, Direction dir_); //�ړ��ʐݒ�
		void Move(); //�ړ�
		bool IsNear(ML::Vec2 pos_); //�v���C���[�m�F
		bool IsPushable(ML::Vec2 next_); //�ړ���m�F
	};
}