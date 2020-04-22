#include "LevelLoader.h"
#include "Wall.h"
#include "MathHelperFunctions.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Texture.h"
#include "Surface.h"
#include <jsoncons/json.hpp>
#include <iostream>
#include <fstream>
#include "GenericModel.h"
#include "WindowObject.h"
#include "Door.h"

bool LevelLoader::Load(std::list<std::unique_ptr<class Drawable>>& drawables, Graphics& graphics) noexcept
{
	bool isLoadingMultiple = false;
	int scalingFactor = 1;
	float wallSize = 2.0;
	float wallHeight = 20;

	bool firstWall = true;
	int objectNumber = 0;
	int renderOriginX = -140;
	int renderOriginZ = -120;
	float renderOriginY = -90.0f;

	LoadLevelFromJson();

	//Load all walls
	for (auto& wall : blueprintData)
	{
		std::unique_ptr newWall = std::make_unique<Wall>(graphics, "cube.tga");

		newWall->SetPos(renderOriginX + (wall.centerPointX / scalingFactor), 
						renderOriginY, 
						renderOriginZ + (wall.centerPointY / scalingFactor));

		//Determine what angle wall is protruding in and rotate & expand
		if ((wall.endX - wall.startX) / scalingFactor > (wall.endY - wall.startY) / scalingFactor)
		{
			newWall->SetRotation((-wall.angle * std::_Pi) / 180);
			newWall->SetScaling((wall.endX - wall.startX) / scalingFactor, wallHeight, wallSize);
		}
		else
		{
			newWall->SetRotation((-wall.angle * std::_Pi) / 180);
			newWall->SetScaling((wall.endY - wall.startY) / scalingFactor, wallHeight, wallSize);
		}

		drawables.emplace_back(
			std::move(newWall));

		objectNumber++;
	}

	//Load doors and windows
	if (shouldLoadSymbols)
	{
		LoadSymbolsFromJson();

		for (auto& symbol : symbolData)
		{
			//Code duplication here, this was initally done because the window and door objs may have had different base/default scaling so would have to be scaled accordingly
			if (symbol.objectName == "window")
			{
				std::unique_ptr newWindow = std::make_unique<WindowObject>(graphics);

				newWindow->SetPos(renderOriginX + (symbol.xPos / scalingFactor), 
								  renderOriginY,
								  renderOriginZ + (symbol.yPos / scalingFactor));

				//Find nearest wall and assume this has the same rotation
				newWindow->SetRotation((FindNearestWallAngleToSymbol(symbol.xPos, symbol.yPos)
										* std::_Pi) / 180);

				POINT NearestWallEnd = FindNearestWallEnd(symbol.xPos, symbol.yPos);
				POINT NearestWallStart = FindNearestWallStart(symbol.xPos, symbol.yPos);

				if ((NearestWallEnd.x - NearestWallStart.x) / scalingFactor > (NearestWallEnd.y - NearestWallStart.y) / scalingFactor)
				{
					newWindow->SetScaling((NearestWallEnd.x - NearestWallStart.x) / scalingFactor,
										   wallHeight, 
										   wallSize);
				}
				else
				{
					newWindow->SetScaling((NearestWallEnd.y - NearestWallStart.y) / scalingFactor, wallHeight, wallSize);
				}

				drawables.emplace_back(std::move(newWindow));
			}
			else if(symbol.objectName == "door")
			{
				std::unique_ptr newDoor = std::make_unique<Door>(graphics);

				newDoor->SetPos(renderOriginX + (symbol.xPos / scalingFactor),
					renderOriginY,
					renderOriginZ + (symbol.yPos / scalingFactor));

				//Find nearest wall and assume this has the same rotation
				newDoor->SetRotation((FindNearestWallAngleToSymbol(symbol.xPos, symbol.yPos)
					* std::_Pi) / 180);

				POINT NearestWallEnd = FindNearestWallEnd(symbol.xPos, symbol.yPos);
				POINT NearestWallStart = FindNearestWallStart(symbol.xPos, symbol.yPos);

				if ((NearestWallEnd.x - NearestWallStart.x) / scalingFactor > (NearestWallEnd.y - NearestWallStart.y) / scalingFactor)
				{
					newDoor->SetScaling((NearestWallEnd.x - NearestWallStart.x) / scalingFactor,
						wallHeight,
						wallSize);
				}
				else
				{
					newDoor->SetScaling((NearestWallEnd.y - NearestWallStart.y) / scalingFactor, wallHeight, wallSize);
				}

				drawables.emplace_back(std::move(newDoor));
			}
		}
	}

	hasLevelLoaded = true;
	return true;
}


void LevelLoader::SaveObj(std::list<std::unique_ptr<class Drawable>>& drawables, Graphics& graphics) noexcept
{
	std::ofstream objFile;
	objFile.open("objFile.obj");
	bool firstWall = true;

	int currentDisplacementamount = 0;
	int objectNumber = 0;

	for (auto& d : drawables)
	{
		if (!d->ShouldBeDeleted())
		{
			objFile << d->GetObjFileString(currentDisplacementamount, objectNumber, graphics);
			currentDisplacementamount += d->GetNumberOfVerticies();
			objectNumber += 1;
		}
	}
}

void LevelLoader::SpawnObject(Spawnables spawnable, DirectX::XMFLOAT3 position, Graphics& graphics, std::list<std::unique_ptr<class Drawable>>& drawables) noexcept
{
	if (spawnable == Spawnables::WINDOW)
	{
		std::unique_ptr newWindow = std::make_unique<WindowObject>(graphics);
		newWindow->SetPos(position.x, -90.0f, position.z);
		newWindow->SetScaling(15.0f, 10.0f, 15.0f);
		drawables.emplace_back(std::move(newWindow));
	}
	else if (spawnable == Spawnables::DOOR)
	{
		std::unique_ptr newDoor = std::make_unique<Door>(graphics);
		newDoor->SetPos(position.x, -100.0f, position.z);
		newDoor->SetScaling(2.0f, 0.26f, 2.0f);
		drawables.emplace_back(std::move(newDoor));
	}
	else
	{
		std::unique_ptr newWall = std::make_unique<Wall>(graphics, "cube.tga");
		newWall->SetPos(position.x, -90.0f, position.z);
		newWall->SetScaling(20.0f, 20.0f, 2.0f);
		drawables.emplace_back(std::move(newWall));
	}
}

void LevelLoader::LoadSymbolsFromJson() noexcept
{
	using jsoncons::json;
	std::ifstream is("Data\\symbols.json");
	json pointJSON;
	is >> pointJSON;

	// Get a reference to symbolsfound array 
	const json& v = pointJSON["symbolsFound"];

	for (const auto& item : v.array_range())
	{
		std::string objectName = item["objectFound"].as_string();
		int enumNumber = item["enumNumber"].as_integer();
		int xPos = item["xPos"].as_integer();
		int yPos = item["yPos"].as_integer();
		symbolData.push_back(SymbolMetaData(objectName, enumNumber, xPos, yPos));
	}

}

POINT LevelLoader::FindNearestWallEnd(int xPos, int yPos)
{
	POINT currentWall;
	int nearestEuclideanDistance = 100000000;
	bool isFirst = true;
	for (auto& wall : blueprintData)
	{
		int distanceToWallEnd = (xPos - wall.endX) ^ 2 + (yPos - wall.endY) ^ 2;

		if (distanceToWallEnd < nearestEuclideanDistance || isFirst)
		{
			nearestEuclideanDistance = distanceToWallEnd;
			currentWall.x = wall.endX;
			currentWall.y = wall.endY;
			isFirst = false;
		}
	}

	return currentWall;
}

POINT LevelLoader::FindNearestWallStart(int xPos, int yPos)
{
	POINT currentWall;
	int nearestEuclideanDistance = 1000000000;
	bool isFirst = true;
	for (auto& wall : blueprintData)
	{
		int distanceToWallStart = (xPos - wall.startX) ^ 2 + (yPos - wall.startY) ^ 2;

		if (distanceToWallStart < nearestEuclideanDistance || isFirst)
		{
			nearestEuclideanDistance = distanceToWallStart;
			currentWall.x = wall.endX;
			currentWall.y = wall.endY;
			isFirst = false;
		}
	}

	return currentWall;
}

//Squared euclidean distance check
//Used to find the angle of the nearest wall to give that object the same rotation
int LevelLoader::FindNearestWallAngleToSymbol(int xPos, int yPos) noexcept
{
	int nearestX;
	int nearestY;
	int nearestEuclideanDistance;
	int wallRotation = 0;
	bool isFirst = true;

	for (auto& wall : blueprintData)
	{
		int distanceToWallEnd = (xPos - wall.endX) ^ 2 + (yPos - wall.endY) ^ 2;
		int distanceToWallStart = (xPos - wall.startX) ^ 2 + (yPos - wall.startY) ^ 2;

		if (isFirst)
		{
			if (distanceToWallEnd < distanceToWallStart)
			{
				nearestEuclideanDistance = distanceToWallEnd;
				wallRotation = wall.angle;
			}
			else
			{
				nearestEuclideanDistance = distanceToWallStart;
				wallRotation = wall.angle;
			}
			isFirst = false;
		}
		else
		{
			if (distanceToWallEnd < nearestEuclideanDistance)
			{
				nearestX = wall.endX;
				nearestY = wall.endY;
				wallRotation = wall.angle;
				nearestEuclideanDistance = distanceToWallEnd;
			}
			else if (distanceToWallStart < nearestEuclideanDistance)
			{
				nearestX = wall.startX;
				nearestY = wall.startY;
				wallRotation = wall.angle;
				nearestEuclideanDistance = distanceToWallStart;
			}
		}
	}

	return wallRotation;
}

void LevelLoader::LoadLevelFromJson() noexcept
{
	using jsoncons::json;
	std::ifstream is("Data\\data.json");
	json pointJSON;
	is >> pointJSON;

	const json& firstArray = pointJSON["metaData"];

	for (const auto& item : firstArray.array_range())
	{
		//Python stores boolean as a string
		std::string should = item["shouldLoadSymbols"].as_string();

		if (should == "False")
		{
			shouldLoadSymbols = false;
		}
		else
		{
			shouldLoadSymbols = true;
		}
	}

	// Get a reference to points array 
	const json& v = pointJSON["points"];

	for (const auto& item : v.array_range())
	{
		int startX = item["startX"].as_integer();
		int startY = item["startY"].as_integer();
		int endX = item["endX"].as_integer();
		int endY = item["endY"].as_integer();
		int centerpointX = item["centerpointX"].as_integer();
		int centerpointY = item["centerpointY"].as_integer();
		int angle = item["angle"].as_integer();
		blueprintData.push_back(WallMetaData(startX, startY, endX, endY, centerpointX, centerpointY, angle));
	}
}


std::vector<WallMetaData> LevelLoader::LoadLevelsFromJson(std::string fileName) noexcept
{
	std::vector<WallMetaData> points;
	using jsoncons::json;
	std::ifstream is("Data\\" + fileName);
	json pointJSON;
	is >> pointJSON;

	// Get a reference to reputons array 
	const json& v = pointJSON["points"];

	for (const auto& item : v.array_range())
	{
		int startX = item["startX"].as_integer();
		int startY = item["startY"].as_integer();
		int endX = item["endX"].as_integer();
		int endY = item["endY"].as_integer();
		int centerpointX = item["centerpointX"].as_integer();
		int centerpointY = item["centerpointY"].as_integer();
		int angle = item["angle"].as_integer();
		points.push_back(WallMetaData(startX, startY, endX, endY, centerpointX, centerpointY, angle));
	}

	return points;
}

