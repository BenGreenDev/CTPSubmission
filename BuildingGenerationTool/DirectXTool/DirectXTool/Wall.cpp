#include "Wall.h"
#include "Texture.h"
#include "Surface.h"
#include "BindableBase.h"
#include "Sampler.h"
#include "imgui.h"

Wall::Wall(Graphics& gfx, std::string fileName)
{
	namespace dx = DirectX;

	if (!IsStaticInitialised())
	{
		model = Cube::MakeSkinned<Vertex>();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		AddStaticBind(std::make_unique<Texture>(gfx, Surface::FromFile("./Images/" + fileName)));
		AddStaticBind(std::make_unique<Sampler>(gfx));

		auto pvs = std::make_unique<VertexShader>(gfx, L"TextureVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"TexturePS.cso"));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	// model deformation transform (per instance, not stored as bind)
	DirectX::XMStoreFloat3x3(
		&mt,
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f)
	);

}

void Wall::SetPos(int _xPos, int _yPos, int _zPos) noexcept
{
	xPos = _xPos;
	yPos = _yPos;
	zPos = _zPos;

	DirectX::XMStoreFloat3x3(&mt, DirectX::XMMatrixTranslation(xPos, yPos, zPos));
}
void Wall::SetScaling(float xScaling, float yScaling, float zScaling)
{
	xScale = xScaling;
	yScale = yScaling;
	zScale = zScaling;

	DirectX::XMStoreFloat3x3(
		&mt,
		DirectX::XMMatrixScaling(xScaling, yScaling, zScaling));
}
void Wall::SetRotation(float rotation)
{
	yaw = rotation;
	not_Radians_Rotation = yaw * (180.0 / std::_Pi);

	DirectX::XMStoreFloat3x3(
		&mt,
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
};

std::string Wall::GetObjFileString(int displacementSize, int objName, Graphics& graphics)
{
	std::string finalString = "";
	std::string verticies = "";
	std::string vertexTextures = "";
	std::string fvalues = "";

	const auto modelView = GetTransformXM();
	const auto modelViewProj = modelView * graphics.GetProjection();
	
	for (auto& vert : model.vertices)
	{
		DirectX::XMFLOAT4 temp = { vert.pos.x, vert.pos.y, vert.pos.z, 1.0f };
		auto tempVec = DirectX::XMLoadFloat4(&temp);

		tempVec = DirectX::XMVector4Transform(tempVec, modelViewProj);
		DirectX::XMStoreFloat4(&temp, tempVec);

		float x = -temp.x;
		float y = temp.y;
		float z = temp.z;


		float u = vert.tex.u;
		float v = vert.tex.v;
		std::string currentVertex = "v  " + std::to_string(x) + "  " + std::to_string(y) + "  " + std::to_string(z) + "\n";
		std::string currentVT = "vt  " + std::to_string(u) + "  " + std::to_string(v) + "  " + "0.0" + "\n";
		verticies.append(currentVertex);
		vertexTextures.append(currentVT);
	}

	for (int i = 0; i < indexBuffer.size(); i++)
	{
		if (i % 3 == 0)
		{
			fvalues.append("\nf ");
			fvalues.append(std::to_string(displacementSize + indexBuffer[i] + 1) + " ");
		}
		else
		{
			fvalues.append(std::to_string(displacementSize + indexBuffer[i] + 1) + " ");
		}
	}

	finalString = verticies + "\n" + "g object" + std::to_string(objName) + "\n" + fvalues + "\n\n";
	return finalString;
}

bool Wall::CheckRayIntersection(DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR rayDirection)
{
	for (int i = 0; i < indexBuffer.size() / 3; ++i)
	{
		DirectX::XMVECTOR v0 = DirectX::XMVector3Transform(DirectX::XMVECTOR{ model.vertices[indexBuffer[i]].pos.x,  
																			  model.vertices[indexBuffer[i]].pos.y, 
																			  model.vertices[indexBuffer[i]].pos.z }, GetTransformXM());
		DirectX::XMVECTOR v1 = DirectX::XMVector3Transform(DirectX::XMVECTOR{ model.vertices[indexBuffer[i + 1]].pos.x,  
																			  model.vertices[indexBuffer[i + 1]].pos.y, 
																			  model.vertices[indexBuffer[i + 1]].pos.z }, GetTransformXM());
		DirectX::XMVECTOR v2 = DirectX::XMVector3Transform(DirectX::XMVECTOR{ model.vertices[indexBuffer[i + 2]].pos.x,  
																		      model.vertices[indexBuffer[i + 2]].pos.y, 
																			  model.vertices[indexBuffer[i + 2]].pos.z }, GetTransformXM());
		
		DirectX::XMVECTOR v0v1 = DirectX::XMVectorSubtract(v1, v0);
		DirectX::XMVECTOR v0v2 = DirectX::XMVectorSubtract(v2, v0);

		DirectX::XMVECTOR pvec = DirectX::XMVector3Cross(rayDirection, v0v2);
		float det = DirectX::XMVectorGetX(DirectX::XMVector3Dot(v0v1, pvec));

		if (fabs(det) < DBL_EPSILON)
		{
			continue;
		}

		float invDet = 1 / det;

		DirectX::XMVECTOR tvec = DirectX::XMVectorSubtract(rayOrigin, v0);

		float u = DirectX::XMVectorGetX(DirectX::XMVectorScale(DirectX::XMVector3Dot(tvec, pvec), invDet));

		if (u < 0 || u > 1)
		{
			continue;
		}

		DirectX::XMVECTOR qvec = DirectX::XMVector3Cross(tvec, v0v1);

		float v = DirectX::XMVectorGetX(DirectX::XMVectorScale(DirectX::XMVector3Dot(rayDirection, qvec), invDet));

		if (v < 0 || u + v > 1)
		{
			continue;
		}

		float t = DirectX::XMVectorGetX(DirectX::XMVectorScale(DirectX::XMVector3Dot(v0v2, qvec), invDet));

		return true;
	}

	return false;
}

bool Wall::SetAsClicked(bool setTo)
{
	return false;
}

void Wall::RenderUI()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(350, 325), ImGuiCond_Always);

	if (ImGui::Begin("Wall"))
	{
		ImGui::Text("Position");

		ImGui::InputFloat("X Position", &xPos, 1.0f, 5.0f);
		ImGui::InputFloat("Y Position", &yPos, 1.0f, 5.0f);
		ImGui::InputFloat("Z Position", &zPos, 1.0f, 5.0f);

		ImGui::Text("Scaling");

		ImGui::InputFloat("X Scale", &xScale, 1.0f, 5.0f);
		ImGui::InputFloat("Y Scale", &yScale, 1.0f, 5.0f);
		ImGui::InputFloat("Z Scale", &zScale, 1.0f, 5.0f);

		if (ImGui::Button("Set Scaling"))
		{
			SetScaling(xScale, yScale, zScale);
		}

		ImGui::Text("Rotation");

		ImGui::InputFloat("Rotation", &not_Radians_Rotation, 1.0f, 5.0f);
		if (ImGui::Button("Set Rotation"))
		{
			SetPos(xPos, yPos, zPos);
			SetRotation((not_Radians_Rotation * std::_Pi) / 180);
			SetScaling(xScale, yScale, zScale);
		}

		if (ImGui::Button("Delete"))
		{
			SetToBeDeleted = true;
		}
	}
	ImGui::End();
}

void Wall::Update(float dt) noexcept
{
}

DirectX::XMMATRIX Wall::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(xPos, yPos, zPos) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);

}

