#pragma once
#include <memory>
#include <string>

struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

class Surface
{

public:
	Surface(unsigned int width, unsigned int height, unsigned int pitch) noexcept;
	Surface(unsigned int width, unsigned int height) noexcept;
	Surface(Surface&& source) noexcept;
	Surface(Surface&) = delete;
	Surface& operator= (Surface&& donor) noexcept;
	Surface& operator= (const Surface&) = delete;
	~Surface();

	static Surface FromFile(const std::string& filename);
private:
	Surface(unsigned int width, unsigned int height, std::unique_ptr<unsigned char[]> pBufferParam) noexcept;
public:
	//Our pixel buffer array that represents each pixel of a texture loaded in
	std::unique_ptr<unsigned char[]> pBuffer;
	unsigned int width;
	unsigned int height;
	unsigned int pitch;

};


