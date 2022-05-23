#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Effect
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("Effect");	//�O���[�v��
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
		//���\�[�X�摜
		DG::Image::SP img;
		
		//�ړ���
		ML::Vec2 pos;					//�ʒu
		ML::Vec2 moveVec;			//���x
		ML::Vec2 gravityVec;	//�d��

		//�p�x
		float angle;
		float addAngle;				//�p�x�ω�
		float fadeMaxAngle;

		//�g�嗦
		float scale;
		float addScale;				//�T�C�Y�ω�
		float maxScale;

		//�����x
		bool addRender; //���Z����
		float alpha;
		float addAlpha;				//�����x�ω�

		//���\�[�X�T�C�Y
		int resSizeX;
		int	resSizeY;
		int drawSizeX;
		int drawSizeY;

		//�A�j���[�V����
		int animno;						//�A�j���V�����R�}��
		int animMax;					//�I���R�}��
		int	animResNum;				//���\�[�X�̍s��
		float addAnim;				//�A�j���V�������Z

		//�J�E���g
		int count;
		int countMax;				//�ő�ێ�����

		string filename;


		bool fade;
	};
}