//--------------------------------------------------------------//
//	"scene_boot.h"												//
//		�V�[��(�N��/�V�[���Z���N�g)								//
//													2023/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class SceneBoot : public vnScene
{
private:
	int Cursor;
	
public:
	bool initialize();
	void terminate();
	
	void execute();
	void render();
};
