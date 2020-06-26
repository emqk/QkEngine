#pragma once


class Texture
{
public:
	Texture();
	~Texture();

	std::string name;

	bool LoadTextureFromFile(const char* path);
	void Use() const;

	std::string type;
	std::string path;

private:
	unsigned int ID;

};