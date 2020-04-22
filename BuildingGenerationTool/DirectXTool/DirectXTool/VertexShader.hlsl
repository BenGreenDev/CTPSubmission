cbuffer CBuf
{
	matrix transform;
};

//We need to return the vertex position, and the vertex colour, hence the struct above.
float4 main(float3 pos : Position) : SV_Position
{
	 return mul(float4(pos, 1.0f), transform);
}