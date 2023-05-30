#pragma once

#include "../object/cube.h"

class SceneModelTest : public vnScene
{
private:
	vnModel* pModel;
	
	float   radius; //îºåa
	float   theta;  //äpìx
	float   phi;	//äpìx

	//ëÄçÏÇÃéÌóﬁ
	enum eOperation
	{
		PositionX,
		PositionY,
		PositionZ,
		RotateX,
		RotateY,
		RotateZ,
		ScaleX,
		ScaleY,
		ScaleZ,
		Radius,
		Theta,
		Phi,
		OperationMax,
	};

	XMVECTOR resetPos;
	XMVECTOR resetRot;
	XMVECTOR resetScl;

	int	Cursor;

public:
	
	bool initialize();
	void terminate();
	
	void execute();
	void render();
};
