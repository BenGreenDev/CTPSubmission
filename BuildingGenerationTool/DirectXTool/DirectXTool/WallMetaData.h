#pragma once
class WallMetaData
{
public:
	WallMetaData(int _startX, int _startY, int _endX, int _endY, int _centerX, int _centerY, int _angle);

public:
	int startX = 0;
	int startY = 0;
	int endX = 0;
	int endY = 0;
	int centerPointX = 0;
	int centerPointY = 0;
	int angle = 0;
};

