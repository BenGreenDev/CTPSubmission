#pragma once
#include <string>

class SymbolMetaData
{
public:
	enum objectType
	{
		DOOR = 0,
		WINDOW = 1
	};

	SymbolMetaData(std::string objectFound, int _enumNumber, int _xPos, int _yPos);

	std::string objectName = "";
	objectType objectID = DOOR;
	int enumNumber;
	int xPos;
	int yPos;
	int rotation;
};

