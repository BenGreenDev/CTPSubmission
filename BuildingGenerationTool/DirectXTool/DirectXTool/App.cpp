#include "App.h"
#include <memory>
#include "imgui.h"
#include "DrawableBase.h"
#include "MathHelperFunctions.h"
#include <iostream>

App::App() : wnd(1280,720,"DirectXTool")
{
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 1280.0f / 720.0f, 0.5f, 5000.0f));

	levelLoader.Load(drawables, wnd.Gfx());
	cam.Reset();
}

App::~App()
{}

int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	if (levelLoader.hasLevelLoaded)
	{
		const auto dt = timer.Mark();

		wnd.Gfx().BeginFrame(0.3f, 0.3f, 0.4f);

		//W
		if (wnd.keyboard.KeyIsPressed(0x57))
		{
			cam.Translate({ 0.0f,1.0f * dt,0.0f });
		}
		//S
		if (wnd.keyboard.KeyIsPressed(0x53))
		{
			cam.Translate({ 0.0f,-1.0f * dt,0.0f });
		}
		//D
		if (wnd.keyboard.KeyIsPressed(0x44))
		{
			cam.Translate({ 1.0f * dt,0.0f,0.0f });
		}
		//A
		if (wnd.keyboard.KeyIsPressed(0x41))
		{
			cam.Translate({ -1.0f * dt,0.0f,0.0f });
		}

		wnd.Gfx().SetCamera(cam.GetMatrix());

		if (wnd.mouse.LeftIsPressed())
		{
			DoRaycast({ wnd.mouse.GetPosX(), wnd.mouse.GetPosY() }, { screenWidth, screenHeight });
		}

		for (auto& d : drawables)
		{
			if (!d->ShouldBeDeleted())
			{
				d->Update(dt);
				d->Draw(wnd.Gfx());
			}
		}

		ImGuiIO& io = ImGui::GetIO();
		 
		if (io.MouseWheel > 0.0f || io.MouseWheel < 0.0f)
		{
			cam.Translate({ 0.0f, 0.0f, io.MouseWheel * dt * mouseScrollSpeed});
		}

		RenderAppImgui();

		if (selectedDrawable != nullptr)
		{
			selectedDrawable->RenderUI();
		}

		// present
		wnd.Gfx().EndFrame();
	}
}


void App::RenderAppImgui()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	ImGui::SetNextWindowPos(ImVec2(1175, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(100, 140), ImGuiCond_Always);

	if (ImGui::Begin("App Controls", NULL, flags))
	{
		if (ImGui::Button("Export OBJ"))
		{
			levelLoader.SaveObj(drawables, wnd.Gfx());
		}

		if (ImGui::Button("Add Window"))
		{
			levelLoader.SpawnObject(LevelLoader::Spawnables::WINDOW, cam.GetPos(), wnd.Gfx(), drawables);
		}

		if(ImGui::Button("Add Door"))
		{
			levelLoader.SpawnObject(LevelLoader::Spawnables::DOOR, cam.GetPos(), wnd.Gfx(), drawables);
		}

		if (ImGui::Button("Add Wall"))
		{
			levelLoader.SpawnObject(LevelLoader::Spawnables::WALL, cam.GetPos(), wnd.Gfx(), drawables);
		}
	}

	ImGui::End();
}

void App::DoRaycast(DirectX::XMINT2 sp, DirectX::XMINT2 screenDims)
{
	DirectX::XMMATRIX camM = cam.GetMatrix();
	DirectX::XMFLOAT4X4 camMatrix;
	DirectX::XMStoreFloat4x4(&camMatrix, camM);

	DirectX::XMMATRIX camI = DirectX::XMMatrixInverse(NULL, camM);
	DirectX::XMFLOAT4X4 camInverse;
	DirectX::XMStoreFloat4x4(&camInverse, camI);

	//Get position of click on screen
	float px = ((2.0f * (float)sp.x) / (float)screenDims.x) - 1.0f;
	float py = (((2.0f * (float)sp.y) / (float)screenDims.y) - 1.0f) * -1.0f;
	
	DirectX::XMMATRIX projM = wnd.Gfx().GetProjection();
	DirectX::XMFLOAT4X4 projMatrix;
	DirectX::XMStoreFloat4x4(&projMatrix, projM);

	px = px / projMatrix._11;
	py = py / projMatrix._22;

	//Get direction ray will go
	DirectX::XMFLOAT3 direction;
	direction.x = (px * camInverse._11) + (py * camInverse._21) + camInverse._31;
	direction.y = (px * camInverse._12) + (py * camInverse._22) + camInverse._32;
	direction.z = (px * camInverse._13) + (py * camInverse._23) + camInverse._33;

	DirectX::XMFLOAT3 vRayOrig;

	//Working ray origin
	vRayOrig = cam.GetPos();
	
	DirectX::XMVECTOR rayorigin;
	rayorigin = DirectX::XMLoadFloat3(&vRayOrig);

	DirectX::XMVECTOR raydirection;
	raydirection = DirectX::XMLoadFloat3(&direction);

	//Testing variables
	/*DirectX::XMFLOAT3 benOrigin;
	DirectX::XMStoreFloat3(&benOrigin, rayori);

	DirectX::XMFLOAT3 benDirect;
	DirectX::XMStoreFloat3(&benDirect, raydirection);*/

	int trueCount = 0;
	for(auto& drawable : drawables)
	{
		if (drawable->CheckRayIntersection(rayorigin, raydirection))
		{
			trueCount = trueCount + 1;
			selectedDrawable = drawable.get();
		}
	}

	if (trueCount == 0)
	{
		selectedDrawable == nullptr;
	}
}


