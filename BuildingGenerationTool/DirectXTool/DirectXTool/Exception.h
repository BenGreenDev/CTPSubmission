#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;

private:
	//Line number our error was thrown
	int line;
	//The file the error was thrown from
	std::string file;
protected:
	//Buffer is used to store the value of GetType and GetOriginString so that it lives beyond the local scope of what() call
	mutable std::string whatBuffer;
};

