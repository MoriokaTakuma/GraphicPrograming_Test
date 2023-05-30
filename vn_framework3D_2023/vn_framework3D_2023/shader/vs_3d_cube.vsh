struct vertexIn
{
    float4 pos : POSITION0;
	float3 nor : NORMAL0;
	float2 tx0 : TEXCOORD0;
};
 
struct vertexOut
{
    float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 tx0 : TEXCOORD0;
};
 
cbuffer ConstantBuffer : register(b0)
{
	float4x4    WVP;		//World * View * Proj
	float4x4	World;		//���[���h�s��
	float4		LightDir;	//���s�����́u�t�v����
	float4		LightCol;   //���s�����̐F
	float4		LightAmb;	//�����̐F
}
 
vertexOut main(vertexIn IN )
{
	vertexOut OUT;

	//���_���W��WVP�s��ŕϊ�
	OUT.pos = mul(IN.pos, WVP);

	//�@���x�N�g�������[���h�s��ŕϊ�
	float3 n = mul(float4(IN.nor,0),World).xyz;

	//���C�e�B���O
	OUT.col.xyz = saturate(dot(normalize(n), LightDir.xyz)); //saturate : �O�`�P�̊ԂɎ��߂�
	OUT.col.xyz *= LightCol.xyz;
	OUT.col.xyz += LightAmb.xyz;
	OUT.col.w = 1;      //alpha�l�͂P�ɐݒ�

	//�e�N�X�`�����W
	OUT.tx0 = IN.tx0;

	return OUT;
}