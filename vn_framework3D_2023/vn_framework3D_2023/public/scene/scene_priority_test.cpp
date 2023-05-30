#include "../../framework.h"
#include "../../framework/vn_environment.h"

//�������֐�
bool ScenePriorityTest::initialize()
{
	//�쐬�ł���X�v���C�g�̍ő吔(�z��̐�)
	//�z��S�̂̃o�C�g����v�f��̃o�C�g���Ŋ���
	SpriteMax = sizeof(pSprite) / sizeof(vnSprite*);

	//�|�C���^�̏�����(NULL�N���A)
	for (int i = 0; i < SpriteMax; i++)
	{
		pSprite[i] = NULL;
	}

	//�쐬�����X�v���C�g�̐�
	SpriteNum = 0;

	return true;
}

//�I���֐�
void ScenePriorityTest::terminate()
{
	//���������X�v���C�g�̍폜
	for (int i = 0; i < SpriteMax; i++)
	{
		if (pSprite[i] == NULL)continue;
		deleteObject(pSprite[i]);
	}
}

//�����֐�
void ScenePriorityTest::execute()
{
	vnFont::print(10, 10, L"Priority Test : %d / %d", SpriteNum, SpriteMax);

	//�}�E�X�̍��N���b�N
	if (vnMouse::trg() == true)
	{
		//�}�E�X�J�[�\���̍��W���擾
		float mx = (float)vnMouse::getX();
		float my = (float)vnMouse::getY();

		int spriteID = getNearestSprite(mx, my, 32.0f);

		if (spriteID >= 0)
		{	//�쐬�ς݂̃X�v���C�g��͂�
			pPick = pSprite[spriteID];
			pickDifX = pPick->posX - mx;
			pickDifY = pPick->posY - my;
			setSpriteColor(pPick, 1.0f, 1.0f, 0.75f);
		}
		else
		{	//�X�v���C�g�̐V�K�쐬

			//�z�񂩂�󂫗v�f��T��
			for (int i = 0; i < SpriteMax; i++)
			{
				if (pSprite[i] != NULL)continue;

				pSprite[i] = createSprite(mx, my);

				//�X�v���C�g�̐���1���₷(increment)
				SpriteNum++;

				//�쐬�����X�v���C�g��͂ݏ�Ԃɂ���
				pPick = pSprite[i];
				pickDifX = 0.0f;
				pickDifY = 0.0f;
				setSpriteColor(pPick, 1.0f, 1.0f, 0.75f);
				break;
			}
		}
	}
	else if (vnMouse::relL())
	{
		//�͂�ł���X�v���C�g�𗣂�
		setSpriteColor(pPick);
		pPick = NULL;
	}
	else if (vnMouse::trgR())
	{
		//�X�v���C�g�̍폜
		float mx = (float)vnMouse::getX();
		float my = (float)vnMouse::getY();

		int spriteID = getNearestSprite(mx, my, 32.0f);

		if (spriteID >= 0)
		{
			if (pPick == pSprite[spriteID])pPick = NULL;

			deleteObject(pSprite[spriteID]);
			pSprite[spriteID] = NULL;
			SpriteNum--;
		}
	}

	//�͂�ł���X�v���C�g�̑���
	if (pPick != NULL)
	{
		pPick->posX = (float)vnMouse::getX() + pickDifX;
		pPick->posY = (float)vnMouse::getY() + pickDifY;
	}

	vnScene::execute();
}

//�`��֐�
void ScenePriorityTest::render()
{
	vnScene::render();
}

//�X�v���C�g���쐬
vnSprite* ScenePriorityTest::createSprite(float x, float y)
{
	int disp = rand() % 10;	//0�`9�܂ł̃����_���l�𓾂�

	//0�`9�̉摜�ɑΉ�����uv���v�Z
	float interval_u = 1.0f / 4.0f;	//�摜���Ƃ̊Ԋu(u����)
	float interval_v = 1.0f / 4.0f;	//�摜���Ƃ̊Ԋu(v����)
	float start_u = (float)(disp % 4) * interval_u;	//�ݒ肵����u�l�̍����̒l
	float start_v = (float)(disp / 4) * interval_v;	//�ݒ肵����v�l�̏㑤�̒l

	//�X�v���C�g�𐶐�
	vnSprite* ret = new vnSprite(x, y, 64.0f, 64.0f,
		L"data/image/number_icon.png",
		start_u, start_u + interval_u, start_v, start_v + interval_v);
	//�`��D�揇�ʂ̐ݒ�
	ret->setRenderPriority(10 - disp);

	//�V�[���֓o�^
	registerObject(ret);

	return ret;
}

//�w����W�ɍł��߂��X�v���C�g���擾
int ScenePriorityTest::getNearestSprite(float x, float y, float limit)
{
	float limitSq = limit * limit;

	float dist = D3D11_FLOAT32_MAX;
	int ret = -1;

	for (int i = 0; i < SpriteMax; i++)
	{
		if (pSprite[i] == NULL)continue;
		float dx = pSprite[i]->posX - x;
		float dy = pSprite[i]->posY - y;
		float dSq = dx * dx + dy * dy;
		if (dSq > limitSq)continue;
		if (dSq < dist)
		{
			dist = dSq;
			ret = i;
		}
	}
	return ret;
}

//�X�v���C�g�̒��_�J���[���ꊇ�ݒ�
void ScenePriorityTest::setSpriteColor(vnSprite* p, float r, float g, float b, float a)
{
	if (!p)return;
	for (int i = 0; i < vnSprite::VertexNum; i++)
	{
		p->vtx[i].r = r;
		p->vtx[i].g = g;
		p->vtx[i].b = b;
		p->vtx[i].a = a;
	}
}
