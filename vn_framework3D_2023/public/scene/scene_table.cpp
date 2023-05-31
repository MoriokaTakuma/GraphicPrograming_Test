//--------------------------------------------------------------//
//	"scene_table.cpp"											//
//		�V�[���e�[�u��											//
//													2023/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

//�N�����̃V�[��
eSceneTable initialScene = eSceneTable::Boot;
//���݂̃V�[��
eSceneTable currentScene = (eSceneTable)-1;
//�؂�ւ��\��̃V�[��
eSceneTable reserveScene = initialScene;


//�e�V�[���̖��O
WCHAR SceneName[(int)eSceneTable::SceneMax][32] =
{
	L"Boot",
	L"Cube Test",
	L"Priority Test",
	L"Joystick Test",
	L"SE Test",
	L"Font Test",
};

//�X�e�[�W�؂�ւ��֐�(�\��)
void switchScene(eSceneTable scene)
{
	reserveScene = scene;
}

//���݂̃X�e�[�W�擾(������)
void switchScene()
{
	if(currentScene==reserveScene)
	{
		return;
	}
	
	vnMainFrame::terminateScene();
	
	vnScene *pScene=NULL;
	switch(reserveScene)
	{
	case Boot:			pScene = new SceneBoot();			break;
	case ModelTest:		pScene = new SceneModelTest();		break;
	case PriorityTest:  pScene = new ScenePriorityTest();   break;
	case JoystickTest:	pScene = new SceneJoystickTest();	break;
	case SeTest:		pScene = new SceneSeTest();			break;
	case FontTest:		pScene = new SceneFontTest();		break;
	}
	
	vnMainFrame::initializeScene(pScene);
	
	currentScene = reserveScene;
}
