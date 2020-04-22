#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "LevelLoader.h"
#include <list>

class App
{
public:
	App();
	int Go();
	~App();
private:
	void DoFrame();
	void RenderAppImgui();
	void DoRaycast(DirectX::XMINT2 sp, DirectX::XMINT2 screenDims);
private:
	int screenWidth = 1280;
	int screenHeight = 720;
	ImguiManager imgui;
	Window wnd;
	Timer timer;
	std::list<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 20;
	const float PI = 3.14159265f;
	Camera cam{{0.0f,80.0f,0.0f},1.55334f,PI / 2.0f };
	int mouseScrollSpeed = 2.0f;

	LevelLoader levelLoader;

	Drawable* selectedDrawable = nullptr;
};

