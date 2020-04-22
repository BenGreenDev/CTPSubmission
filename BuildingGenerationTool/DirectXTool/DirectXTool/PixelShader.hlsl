cbuffer CBuf
{
	float4 face_colors[6];
};

//Generate a unique id for every triangle passed into the pixel shader
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	//return face_colors[(tid / 2) % 6];
	return float4(1.0f, 0.5f, 1.0f, 1.0f);
}