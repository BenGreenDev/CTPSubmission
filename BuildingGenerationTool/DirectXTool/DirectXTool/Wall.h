#pragma once
#include "DrawableBase.h"
#include "Cube.h"

class Wall : public DrawableBase<Wall>
{
public:
	Wall(Graphics& gfx, std::string fileName);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

	void SetPos(int _xPos, int _yPos, int _zPos) noexcept override;
	void SetScaling(float xScaling, float yScaling, float zScaling);
	void SetRotation(float rotation);

	std::string GetObjFileString(int displacementSize, int objName, Graphics& graphics) override;

	bool CheckRayIntersection(DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR rayDirection) override;
	bool SetAsClicked(bool setTo);
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

	std::vector<int> indexBuffer{ 
		0,2,1,   2,3,1,
		4,8,5,   5,8,9,
		2,6,3,   3,6,7,
		4,5,7,   4,7,6,
		2,10,11, 2,11,6,
		12,3,7,  12,7,13 };

	IndexedTriangleList<Vertex> model = Cube::MakeSkinned<Vertex>();
	
	float xPos = 0.0f;
	float yPos = 0.0f;
	float zPos = 0.0f;

	float xScale = 0.0f;
	float yScale = 0.0f;
	float zScale = 0.0f;

	float not_Radians_Rotation = 0.0f;

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

	// model transform
	DirectX::XMFLOAT3X3 mt;
};