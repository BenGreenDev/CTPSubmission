#include "SymbolMetaData.h"

SymbolMetaData::SymbolMetaData(std::string objectFound, int _enumNumber, int _xPos, int _yPos)
{
	objectName = objectFound;
	enumNumber = enumNumber;
	objectID = static_cast<objectType>(enumNumber);
	xPos = _xPos;
	yPos = _yPos;
}
