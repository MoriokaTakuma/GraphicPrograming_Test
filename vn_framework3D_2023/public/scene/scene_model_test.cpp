#include "../../framework.h"
#include "../../framework/vn_environment.h"

//初期化関数
bool SceneModelTest::initialize()
{
	//vnLight::setLightDir()

	pModel  = new vnModel(L"data/model/primitive/",L"sphere.vnm");

	registerObject(pModel);

	radius = 5.0f;
	theta  = 0.0f;
	phi    = 0.0f;

	resetPos = XMVectorSet(pModel->getPositionX(), pModel->getPositionY(), pModel->getPositionZ(), 0.0f);
	resetRot = XMVectorSet(pModel->getRotationX(), pModel->getRotationY(), pModel->getRotationZ(), 0.0f);
	resetScl = XMVectorSet(pModel->getScaleX(), pModel->getScaleY(), pModel->getScaleZ(), 0.0f);

	Cursor = 0;

	return true;
}

//終了関数
void SceneModelTest::terminate()
{
	//オブジェクトの削除
	deleteObject(pModel);
}

//処理関数
void SceneModelTest::execute()
{
	float value_t = 0.01f;
	float value_r = 1.0f * 3.141592f / 180.0f;
	float value_s = 0.01f;

	float value_rd = 0.01f;
	float value_th = 1.0f * 3.141592f / 180.0f;

	if (vnKeyboard::trg(DIK_UP) && --Cursor < 0)
	{
		Cursor = OperationMax - 1;
	}
	else if (vnKeyboard::trg(DIK_DOWN) && ++Cursor >= OperationMax)
	{
		Cursor = 0;
	}
	else if (vnKeyboard::on(DIK_RIGHT))
	{
		switch (Cursor)
		{
		case PositionX:	pModel->addPositionX(value_t);	break;
		case PositionY:	pModel->addPositionY(value_t);	break;
		case PositionZ:	pModel->addPositionZ(value_t);	break;
		case RotateX:	pModel->addRotationX(value_r);	break;
		case RotateY:	pModel->addRotationY(value_r);	break;
		case RotateZ:	pModel->addRotationZ(value_r);	break;
		case ScaleX:	pModel->addScaleX(value_s);		break;
		case ScaleY:	pModel->addScaleY(value_s);		break;
		case ScaleZ:	pModel->addScaleZ(value_s);		break;
		case Radius:	radius += value_rd; break;
		case Theta:     theta  += value_th; break;
		case Phi:         phi  += value_th; break;
		}
	}
	else if (vnKeyboard::on(DIK_LEFT))
	{
		switch (Cursor)
		{
		case PositionX:	pModel->addPositionX(-value_t);	break;
		case PositionY:	pModel->addPositionY(-value_t);	break;
		case PositionZ:	pModel->addPositionZ(-value_t);	break;
		case RotateX:	pModel->addRotationX(-value_r);	break;
		case RotateY:	pModel->addRotationY(-value_r);	break;
		case RotateZ:	pModel->addRotationZ(-value_r);	break;
		case ScaleX:	pModel->addScaleX(-value_s);		break;
		case ScaleY:	pModel->addScaleY(-value_s);		break;
		case ScaleZ:	pModel->addScaleZ(-value_s);		break;
		case Radius:	radius -= value_rd; break;
		case Theta:     theta  -= value_th; break;
		case Phi:          phi -= value_th; break;
		}
	}
	else if (vnKeyboard::on(DIK_SPACE))
	{
		switch (Cursor)
		{
		case PositionX:	pModel->setPositionX(XMVectorGetX(resetPos));	break;
		case PositionY:	pModel->setPositionY(XMVectorGetY(resetPos));	break;
		case PositionZ:	pModel->setPositionZ(XMVectorGetZ(resetPos));	break;
		case RotateX:	pModel->setRotationX(XMVectorGetX(resetRot));	break;
		case RotateY:	pModel->setRotationY(XMVectorGetY(resetRot));	break;
		case RotateZ:	pModel->setRotationZ(XMVectorGetZ(resetRot));	break;
		case ScaleX:	pModel->setScaleX(XMVectorGetX(resetScl));		break;
		case ScaleY:	pModel->setScaleY(XMVectorGetY(resetScl));		break;
		case ScaleZ:	pModel->setScaleZ(XMVectorGetZ(resetScl));		break;
		case Radius:	radius = 5.0f; break;
		case Theta:     theta  = 0.0f; break;
		case Phi:		phi    = 0.0f; break;
		}
	}

	//極座標を直交座標に変換
	float tx = cosf(theta) * radius;
	float tz = sinf(theta) * radius;

	float px = cosf(theta) * cosf(phi) * radius;
	float py = sinf(phi)  * radius;
	float pz = sin(theta) * cosf(phi) * radius;

	vnCamera::setPosition(px, py, pz);

	/*
	マウス操作で,theta,phi,radiusの変数を操作できるようにする
	Alt + 左ドラッグ	：　カメラ回転
	Alt + 右ドラッグ	：	カメラ距離(遠近)

	※phiの値は±90°以内に収めること
	※radiusの値はゼロ以下にしないこと
	*/
	
	float mouseX = vnMouse::getDX();
	float mouseY = vnMouse::getDY();

	

	if (vnMouse::onL() && vnKeyboard::on(DIK_LALT))
	{
		 //radius += value_rd; 
		theta += mouseX / 150;
		phi   += mouseY / 150;
		
		// phiの値を制限する
		if (phi >= 80.0f)
			phi = 80.0f;
		else if (phi <= -70.0f)
			phi = -70.0f;
	}

	if (vnMouse::onR() && vnKeyboard::on(DIK_LALT))
	{
		radius += mouseX / 200;

		if (radius <= 1.0f) radius = 1.0f;
	}

	int line = 0;
	vnFont::print(80.0f, (float)(100 + Cursor * 16), L"→");
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"PositionX : %.3f", pModel->getPositionX());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"PositionY : %.3f", pModel->getPositionY());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"PositionZ : %.3f", pModel->getPositionZ());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"RotationX : %.3f", pModel->getRotationX() / 3.141592f * 180.0f);
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"RotationY : %.3f", pModel->getRotationY() / 3.141592f * 180.0f);
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"RotationZ : %.3f", pModel->getRotationZ() / 3.141592f * 180.0f);
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"ScaleX : %.3f", pModel->getScaleX());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"ScaleY : %.3f", pModel->getScaleY());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"ScaleZ : %.3f", pModel->getScaleZ());
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"Radius : %.3f", radius);
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"Theta  : %.3f", theta / 3.141592f * 180.0f);
	vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"Phi    : %.3f", phi / 3.141592f * 180.0f);

	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnScene::execute();
}

//描画関数
void SceneModelTest::render()
{
	vnScene::render();
}
