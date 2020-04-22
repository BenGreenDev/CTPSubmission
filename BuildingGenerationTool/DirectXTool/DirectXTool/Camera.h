#pragma once
#include "Graphics.h"

class Camera
{
public:
	Camera(DirectX::XMFLOAT3 homePos = { 0.0f,0.0f,0.0f }, float homePitch = 0.0f, float homeYaw = 0.0f) noexcept;

	DirectX::XMMATRIX GetMatrix() const noexcept;

	void Reset() noexcept;

	void Translate(DirectX::XMFLOAT3 translation) noexcept;

	DirectX::XMFLOAT3 GetPos() const noexcept;
	void Rotate(float dx, float dy) noexcept;

private:
	const float PI = 3.14159265f;
	const double PI_D = 3.1415926535897932;

	bool canCameraMove = true;

	float movementSpeedMultiplier = 0.5f;

	DirectX::XMFLOAT3 homePos;
	float homePitch;
	float homeYaw;
	DirectX::XMFLOAT3 pos;
	float pitch;
	float yaw;
	static constexpr float travelSpeed = 100.0f;
	static constexpr float rotationSpeed = 0.004f;


	template<typename T>
	T wrap_angle(T theta) noexcept
	{
		const T twoPi = (T)2 * (T)PI_D;
		const T mod = fmod(theta, twoPi);
		if (mod > (T)PI_D)
		{
			return mod - twoPi;
		}
		else if (mod < -(T)PI_D)
		{
			return mod + twoPi;
		}
		return mod;
	}

};