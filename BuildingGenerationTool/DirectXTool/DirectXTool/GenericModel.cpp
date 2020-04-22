//#include "GenericModel.h"
//#include "GenericOBJ.h"
//#include "Texture.h"
//#include "Surface.h"
//#include "BindableBase.h"
//#include "Sampler.h"
//
//struct Vertex
//{
//	DirectX::XMFLOAT3 pos;
//};
//
//GenericModel::GenericModel(Graphics& gfx,
//	std::string fileName)
//	:
//	TestObject(gfx)
//{
//	namespace dx = DirectX;
//
//	if (!IsStaticInitialised())
//	{
//		auto model = GenericOBJ::MakeOBJ<Vertex>(fileName.c_str(), false);
//		
//		//model.SetNormalsIndependentFlat();
//
//		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
//		/*AddStaticBind(std::make_unique<Texture>(gfx, Surface::FromFile("./Images/cube.tga")));
//		AddStaticBind(std::make_unique<Sampler>(gfx));*/
//
//		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));
//
//		auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
//		auto pvsbc = pvs->GetBytecode();
//		AddStaticBind(std::move(pvs));
//
//		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));
//
//		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
//		{
//			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
//			//{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
//		};
//
//		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
//		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
//	}
//	else
//	{
//		SetIndexFromStatic();
//	}
//
//	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
//}
//
//
//void GenericModel::SetPos(int _xPos, int _yPos, int _zPos) noexcept
//{
//}