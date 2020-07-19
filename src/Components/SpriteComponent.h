#pragma once
#include "Component.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../Mesh.h"


class SpriteComponent : public Component
{
public:
	SpriteComponent(GameObject* _parent);
	~SpriteComponent();

	SpriteComponent(const SpriteComponent& comp);
	SpriteComponent& operator=(const SpriteComponent& comp) = delete;

	void Update(const float& deltaTime) override;
	void LateUpdate(const float& deltaTime) override;

	void ShowOnInspector() override;
	
	void SetMeshNew(const char* meshPath);
	Mesh* GetMeshNew();

	void SetShader(Shader* _shader);
	void SetShader(const char* shaderPath);
	Shader* GetShader();

	void SetTexture(const char* texPath);
	void SetTexture(Texture* tex);
	const Texture* GetTexture();

	glm::vec4 color;
	glm::vec3 specular{0.5, 0.5f, 0.5f};
	float shininess = 32;

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

private:
	Mesh* meshNew = nullptr;
	Shader* shader = nullptr;
	Texture* texture = nullptr;
};