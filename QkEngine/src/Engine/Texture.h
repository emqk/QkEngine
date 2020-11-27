#pragma once
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	std::string name;

	bool LoadTextureFromFile(const char* path);
	void Use() const;

	unsigned int GetID() const;

	std::string type;

private:
	unsigned int ID;

};