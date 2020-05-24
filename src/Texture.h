#pragma once


class Texture
{
public:
	Texture();
	~Texture();

	std::string name;

	bool LoadTextureFromFile(const char* path);
	void Use() const;

private:
	unsigned int texture;

};