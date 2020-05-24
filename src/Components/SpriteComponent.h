#pragma once
#include "Component.h"
#include "../Mesh.h"
#include "../Shader.h"
#include "../Texture.h"


class SpriteComponent : public Component
{
public:
	SpriteComponent(GameObject* _parent);
	~SpriteComponent();

	SpriteComponent(const SpriteComponent& comp);
	SpriteComponent& operator=(const SpriteComponent& comp) = delete;

	void Update(const float& deltaTime) override;
	void ShowOnInspector() override;

	void SetMesh(const char* meshPath);
	Mesh* GetMesh();

	void SetShader(Shader* _shader);
	void SetShader(const char* shaderPath);
	Shader* GetShader();

	void SetTexture(const char* texPath);
	void SetTexture(Texture* tex);
	const Texture* GetTexture();

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

private:
	Mesh* mesh = nullptr;
	Shader* shader = nullptr;
	Texture* texture = nullptr;

};