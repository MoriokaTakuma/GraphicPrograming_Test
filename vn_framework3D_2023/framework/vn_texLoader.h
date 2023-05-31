//--------------------------------------------------------------//
//	"vn_texLoader.h"											//
//		�e�N�X�`���ǂݍ��݊Ǘ��N���X							//
//													2023/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//�e�N�X�`���̍ő吔
#define vnTEXTURE_LOADER_MAX	(256)

class vnTexLoader
{
private:
	//�e�N�X�`���P���̏���ێ�����N���X
	class cTexLoader
	{
	public:
		//�t�@�C���̃p�X
		WCHAR	path[256];

		//�Q�Ɛ�
		int refCount;

		//�ǂݍ��܂ꂽ�e�N�X�`��
		ID3D12Resource* texbuff;
		DXGI_FORMAT format;

		cTexLoader()
		{
			init();
		}

		void init()
		{
			memset(path, 0, sizeof(path));
			refCount = 0;
			texbuff = NULL;
			format = DXGI_FORMAT_UNKNOWN;
		}
	};
	static cTexLoader	texLoader[vnTEXTURE_LOADER_MAX];

public:
	//������
	static bool initialize(void);
	//�I��
	static void terminate(void);

	//�e�N�X�`���̓ǂݍ���
	static HRESULT load(const WCHAR *path, ID3D12Resource **ppTexture, DXGI_FORMAT *format);

	//�e�N�X�`���̉��
	static void release(ID3D12Resource *p);
};
