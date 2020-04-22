#pragma once
#include "EasierWindowDotHInclude.h"
#include "Exception.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <optional>
#include <memory>

class Window
{

public:
	class ExceptionHandler : public Exception
	{
		using Exception::Exception;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};
private:
	//Create a singleton for easier means of fetching the window, and interacting with it.
	class WindowClass {
	public:
		//Will return wndClassName
		static const char* GetName() noexcept;
		//Will return hInst;
		static HINSTANCE GetInstance() noexcept;
	private:
		//Constructor for registering
		WindowClass() noexcept;
		//Destructor
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Default Window";
		//Static instance will be created on program start
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	//Constructor / destructor
	Window(int _width, int _height, const char* _name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static std::optional<int> ProcessMessages() noexcept;

	//Means of accessing embeded graphics
	Graphics& Gfx();
	int GetWidth();
	int GetHeight();
private:
	//Static to register as a windows procedure
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	//Will handle the messages for this window
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Keyboard keyboard;
	Mouse mouse;
private:
	int width;
	int height;
	HWND hWnd;
	//We need to differ the creation of gfx, since hWnd isn't accessible outside the body of the constructor, since it is made in its body. We cant instantiate gfx without a reference to it.
	std::unique_ptr<Graphics> pGfx;
};

