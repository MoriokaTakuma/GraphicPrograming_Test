//--------------------------------------------------------------//
//	"scene_table.h"												//
//		�V�[���e�[�u��											//
//													2023/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//�V�[���̎��
enum eSceneTable
{
	Boot,
	ModelTest,
	PriorityTest,
	JoystickTest,
	SeTest,
	FontTest,
	SceneMax,
};

//�V�[���N���X��`�t�@�C��
#include "vn_scene.h"
#include "scene_boot.h"
#include "scene_model_test.h"
#include "scene_priority_test.h"
#include "scene_joystick_test.h"
#include "scene_se_test.h"
#include "scene_font_test.h"

//�V�[���؂�ւ��֐�(�\��)
void switchScene(eSceneTable scene);
//���݂̃V�[���擾(������)
void switchScene();