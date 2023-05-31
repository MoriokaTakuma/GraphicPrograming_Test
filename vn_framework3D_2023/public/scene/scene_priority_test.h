#pragma once

class ScenePriorityTest : public vnScene
{
private:
	vnSprite* pSprite[512];	//�X�v���C�g�̔z��
	int			SpriteMax;		//�쐬�ł���X�v���C�g�̍ő吔
	int			SpriteNum;		//�쐬�����X�v���C�g�̐�

	//�͂ݏ��
	vnSprite* pPick;
	float		pickDifX;
	float		pickDifY;

	//�X�v���C�g���쐬
	vnSprite* createSprite(float x, float y);

	//�w����W�ɍł��߂��X�v���C�g���擾(limit : �X�v���C�g����̗L������)
	int getNearestSprite(float x, float y, float limit);

	//�X�v���C�g�̒��_�J���[���ꊇ�ݒ�
	void setSpriteColor(vnSprite* p, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

public:
	//������
	bool initialize();
	//�I��
	void terminate();

	//����
	void execute();
	//�`��
	void render();
};
