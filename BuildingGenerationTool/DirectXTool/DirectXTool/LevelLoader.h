#pragma once
#include "Window.h"
#include "WallMetaData.h"
#include "SymbolMetaData.h"
#include <list>

class LevelLoader
{
public:
	enum Spawnables
	{
		DOOR,
		WINDOW,
		WALL
	};

	bool Load(std::list<std::unique_ptr<class Drawable>>& drawables, Graphics& gfx) noexcept;

	bool hasLevelLoaded = false;
	void SaveObj(std::list<std::unique_ptr<class Drawable>>& drawables, Graphics& graphics) noexcept;
	void SpawnObject(Spawnables spawnable, DirectX::XMFLOAT3 position, Graphics& graphics, std::list<std::unique_ptr<class Drawable>>& drawables) noexcept;

private:
	void LoadSymbolsFromJson() noexcept;

	//These two functions are used to find where a window/door fits between two walls
	POINT FindNearestWallEnd(int xPos, int yPos);
	POINT FindNearestWallStart(int xPos, int yPos);
	int FindNearestWallAngleToSymbol(int xPos, int yPos) noexcept;

	void LoadLevelFromJson() noexcept;
	std::vector<WallMetaData> LoadLevelsFromJson(std::string fileName) noexcept;

	std::vector<WallMetaData> blueprintData;
	std::vector<SymbolMetaData> symbolData;
	std::vector<std::vector<WallMetaData>> entireLevelData;

	bool shouldLoadSymbols = false;
};

