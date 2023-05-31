//--------------------------------------------------------------//
//	"vn_texLoader.cpp"											//
//		�e�N�X�`���ǂݍ��݊Ǘ��N���X							//
//													2023/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "vn_environment.h"

//�ÓI�ϐ�
vnTexLoader::cTexLoader vnTexLoader::texLoader[vnTEXTURE_LOADER_MAX];

//������
bool vnTexLoader::initialize(void)
{
	for (int i = 0; i < vnTEXTURE_LOADER_MAX; i++)
	{
		texLoader[i].init();
	}
	return true;
}

//�I��
void vnTexLoader::terminate(void)
{
	for (int i = 0; i < vnTEXTURE_LOADER_MAX; i++)
	{
		SAFE_RELEASE(texLoader[i].texbuff);
		texLoader[i].init();
	}
}

//�e�N�X�`���̓ǂݍ���
HRESULT vnTexLoader::load(const WCHAR* path, ID3D12Resource** ppTexture, DXGI_FORMAT* format)
{
	if (path == NULL || ppTexture == NULL)return E_FAIL;

	int id = -1;	//�V�K�œǂݍ��ގ��̔z��ԍ�

	//�����̃e�N�X�`��������
	for (int i = 0; i < vnTEXTURE_LOADER_MAX; i++)
	{
		if (wcscmp(texLoader[i].path,path) == 0)
		{
			texLoader[i].path;
		}
		else
		{
			id = i;
		}


		/*
		
		�����̃e�N�X�`�������łɓǂݍ��ݍς݂̏ꍇ�A�V���ɓǂݍ��݂������A���łɓǂݍ��܂ꂽ�|�C���^��Ԃ�
		if(texLoader[i].path , path���r)
		{
			���łɓǂݍ��܂ꂽ�|�C���^��Ԃ�
		}
		else
		{
			id = i;
		}
		�e�N�X�`�����܂��ǂݍ��܂�Ă��Ȃ��ꍇ�́A�ǂݍ��ݗ\��̔z��ԍ����o���Ă���

		*/
	}

	if (id == -1)return E_FAIL;

	//�e�N�X�`���̐V�K�ǂݍ���

	HRESULT hr = S_OK;
	ID3D12Resource* texbuff = NULL;
	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	const WCHAR* ext = wcschr(path, L'.');
	if (ext == NULL)return E_FAIL;

	if (wcscmp(ext, L".tga") == 0 || wcscmp(ext, L".TGA") == 0)
	{	//tga
		hr = LoadFromTGAFile(path, TGA_FLAGS_NONE, &metadata, scratchImg);
	}
	else
	{	//png, jpg, bmp
		hr = LoadFromWICFile(path, WIC_FLAGS_NONE, &metadata, scratchImg);
	}
	if (hr != S_OK)
	{
		*ppTexture = NULL;
		return hr;
	}
	const Image* img = scratchImg.GetImage(0, 0, 0);

	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapProp.CreationNodeMask = 0;
	texHeapProp.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = metadata.format;
	resDesc.Width = metadata.width;
	resDesc.Height = (UINT)metadata.height;
	resDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.MipLevels = (UINT16)metadata.mipLevels;
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	hr = vnDirect3D::getDevice()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		NULL,
		IID_PPV_ARGS(&texbuff)
	);
	assert(hr == S_OK);

	hr = texbuff->WriteToSubresource(0,
		NULL,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);
	assert(hr == S_OK);//���s����΃v���O�������t���[�Y������...assert

	//�ǂݍ��񂾃e�N�X�`�����̊m��
	/*

	�ǂݍ��ݗ\��̔z��ԍ��̍\���̂ցA���A�����e�N�X�`�����ǂݍ��܂ꂽ�ꍇ�Ɏg���܂킹��悤�ɁA�����o���Ă���

	texLoader[id].path

	*/

	//�Ăяo�����֕ԋp

	/*
	
	�V�K�ɓǂݍ��񂾃e�N�X�`���̏��(�|�C���^)���Ăяo�����֕Ԃ�

	*ppTexture =
	*format =  metadata.format;

	*/


	return hr;
}

//�e�N�X�`���̉��
void vnTexLoader::release(ID3D12Resource* p)
{
	if (p == NULL)return;

	/*
	
	�@���������e�N�X�`����z��̒����猩����

	�A�Q�Ɛ���1���炷

	�B-1 : �Q�Ɛ����c���Ă���ꍇ�́A���̃e�N�X�`���͕ʂ̃I�u�W�F�N�g���܂��g���Ă����ԂȂ̂ŁA�������Ȃ�

	�B-2 : �Q�Ɛ����[���ɂȂ����ꍇ�A���̃e�N�X�`�����g���Ă���I�u�W�F�N�g�����݂��Ȃ��̂ŁA�e�N�X�`�����̂��폜(Release)����
	       ��cTexLoader�̊Y���X���b�g�͍ė��p�ł���悤�ɁA���g���N���A���Ă���
	
	*/
}


