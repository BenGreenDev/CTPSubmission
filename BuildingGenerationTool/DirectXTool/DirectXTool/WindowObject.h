#pragma once
#include "DrawableBase.h"
#include "GenericOBJ.h"

class WindowObject : public DrawableBase<WindowObject>
{
public:
	WindowObject(Graphics& gfx);
	
	void SetPos(int _xPos, int _yPos, int _zPos) noexcept override;
	void SetScaling(float xScaling, float yScaling, float zScaling);
	void SetRotation(float rotation);

	std::string GetObjFileString(int displacementSize, int objName, Graphics& graphics);

	void Update(float dt) noexcept;

	DirectX::XMMATRIX GetTransformXM() const noexcept;
	
	bool CheckRayIntersection(DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR rayDirection) override;

	void RenderUI() override;

	virtual int GetNumberOfVerticies() override
	{
		return model.vertices.size();
	};

private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;

		struct {
			float u;
			float v;
		}tex;
	};

	float xPos = 0.0f;
	float yPos = 0.0f;
	float zPos = 0.0f;

	float xScale = 0.0f;
	float yScale = 0.0f;
	float zScale = 0.0f;

	float not_Radians_Rotation = 0.0f;

	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;

	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;

	IndexedTriangleList<Vertex> model = GenericOBJ::MakeOBJ<Vertex>("./Models/Window/model.obj", false);

	// model transform
	DirectX::XMFLOAT3X3 mt;
};