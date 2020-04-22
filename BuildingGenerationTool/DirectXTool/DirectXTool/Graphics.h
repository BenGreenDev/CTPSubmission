#pragma once
#include "EasierWindowDotHInclude.h"
#include "Exception.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <memory>
#include <random>
#include <DirectXMath.h>

//Every window requires an instance of this
class Graphics
{
	friend class Bindable;

public:
	class ExceptionHandler : public Exception
	{
		using Exception::Exception;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};

public:
	//Our graphics instatiation needs a reference to the window so that it knows what it is drawing too.
	Graphics(HWND hWnd);
	//Remove copying and moving
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	//Flip back buffer and front buffer
	void EndFrame();

	void BeginFrame(float red, float green, float blue) noexcept;

	void DrawIndexed(UINT count) noexcept;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetCamera(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;

	void EnableImgui() noexcept;
	void DisableImgui() noexcept;
	bool IsImguiEnabled() const noexcept;

private:
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;
	bool imguiEnabled = true;

	//Our d3d device for initialising things
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	//Our swapchain for placing the backbuffer in window
	Microsoft::WRL::ComPtr <IDXGISwapChain> pSwap;
	//Our device context used for drawing and creating objects
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> pContext;
	//Our backbuffer renderTarget
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pTarget;
	//Our depth stensil for ensuring things render correctly with depth.
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};

