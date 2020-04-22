#include "Surface.h"
#include <d3d11.h>
#include <stdio.h>
#include "GraphicsThrowMacros.h"

Surface::~Surface()
{
}

Surface::Surface(unsigned int width, unsigned int height, unsigned int pitch) noexcept : height(height), width(width), pitch(pitch)
{}

Surface::Surface(unsigned int width, unsigned int height) noexcept : width(width), height(height)
{}

Surface::Surface(unsigned int width, unsigned int height, std::unique_ptr<unsigned char[]> pBufferParam) noexcept
	: width(width), height(height), pBuffer(std::move(pBufferParam))
{
}

Surface& Surface::operator=(Surface&& donor) noexcept
{
	width = donor.width;
	height = donor.height;
	pBuffer = std::move(donor.pBuffer);
	donor.pBuffer = nullptr;
	return *this;
}

Surface::Surface(Surface && source) noexcept : pBuffer(std::move(source.pBuffer)), width(source.width), height(source.height)
{}

Surface Surface::FromFile(const std::string & filename)
{
	FILE* file;
	int error;
	int count;
	TargaHeader targaHeader;
	unsigned char* targaImage;

	//Open target file
	error = fopen_s(&file, filename.data(), "rb");
	if (error != 0)
	{
		
	}
	//Read in file header
	count = (unsigned int)fread(&targaHeader, sizeof(TargaHeader), 1, file);
	if (count != 1)
	{

	}

	int height = (int)targaHeader.height;
	int width = (int)targaHeader.width;

	int imageSize = (width * height) * 4;
	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{

	}

	count = (unsigned int)fread(targaImage, 1, imageSize, file);
	if (count != imageSize)
	{

	}

	error = fclose(file);


	int index = 0;
	int k = (width * height * 4) - (width * 4) + (9 * 4);

	//Allocate memory for targa destination data
	std::unique_ptr<unsigned char[]> tempBuffer = std::make_unique<unsigned char[]>(imageSize);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			tempBuffer[index + 0] = targaImage[k + 2];
			tempBuffer[index + 1] = targaImage[k + 1];
			tempBuffer[index + 2] = targaImage[k + 0];
			tempBuffer[index + 3] = targaImage[k + 3];

			k += 4;
			index += 4;
		}

		k -= (width * 8);
	}

	delete[] targaImage;
	targaImage = 0;

	return Surface(width, height, std::move(tempBuffer));
}
