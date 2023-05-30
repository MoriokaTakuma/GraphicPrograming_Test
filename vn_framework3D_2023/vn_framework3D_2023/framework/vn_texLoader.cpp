//--------------------------------------------------------------//
//	"vn_texLoader.cpp"											//
//		テクスチャ読み込み管理クラス							//
//													2023/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "vn_environment.h"

//静的変数
vnTexLoader::cTexLoader vnTexLoader::texLoader[vnTEXTURE_LOADER_MAX];

//初期化
bool vnTexLoader::initialize(void)
{
	for (int i = 0; i < vnTEXTURE_LOADER_MAX; i++)
	{
		texLoader[i].init();
	}
	return true;
}

//終了
void vnTexLoader::terminate(void)
{
	for (int i = 0; i < vnTEXTURE_LOADER_MAX; i++)
	{
		SAFE_RELEASE(texLoader[i].texbuff);
		texLoader[i].init();
	}
}

//テクスチャの読み込み
HRESULT vnTexLoader::load(const WCHAR* path, ID3D12Resource** ppTexture, DXGI_FORMAT* format)
{
	if (path == NULL || ppTexture == NULL)return E_FAIL;

	int id = -1;	//新規で読み込む時の配列番号

	//既存のテクスチャを検索
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
		
		同名のテクスチャがすでに読み込み済みの場合、新たに読み込みをせず、すでに読み込まれたポインタを返す
		if(texLoader[i].path , pathを比較)
		{
			すでに読み込まれたポインタを返す
		}
		else
		{
			id = i;
		}
		テクスチャがまだ読み込まれていない場合は、読み込み予定の配列番号を覚えておく

		*/
	}

	if (id == -1)return E_FAIL;

	//テクスチャの新規読み込み

	HRESULT hr = S_OK;
	ID3D12Resource* texbuff = NULL;
	//WICテクスチャのロード
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
	assert(hr == S_OK);//失敗すればプログラムをフリーズさせる...assert

	//読み込んだテクスチャ情報の確保
	/*

	読み込み予定の配列番号の構造体へ、次、同じテクスチャが読み込まれた場合に使いまわせるように、情報を覚えておく

	texLoader[id].path

	*/

	//呼び出し元へ返却

	/*
	
	新規に読み込んだテクスチャの情報(ポインタ)を呼び出し元へ返す

	*ppTexture =
	*format =  metadata.format;

	*/


	return hr;
}

//テクスチャの解放
void vnTexLoader::release(ID3D12Resource* p)
{
	if (p == NULL)return;

	/*
	
	①解放させるテクスチャを配列の中から見つける

	②参照数を1減らす

	③-1 : 参照数が残っている場合は、そのテクスチャは別のオブジェクトがまだ使っている状態なので、何もしない

	③-2 : 参照数がゼロになった場合、そのテクスチャを使っているオブジェクトが存在しないので、テクスチャ自体も削除(Release)する
	       ※cTexLoaderの該当スロットは再利用できるように、中身をクリアしておく
	
	*/
}


