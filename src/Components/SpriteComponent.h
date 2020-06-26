#pragma once
#include "Component.h"
#include "../Mesh.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../MeshNew.h"


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
	MeshNew* GetMeshNew();
	void SetMesh(const char* meshPath);
	Mesh* GetMesh();

	void SetShader(Shader* _shader);
	void SetShader(const char* shaderPath);
	Shader* GetShader();

	void SetTexture(const char* texPath);
	void SetTexture(Texture* tex);
	const Texture* GetTexture();

	glm::vec4 color;

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

private:
	MeshNew* meshNew = nullptr;
	Mesh* mesh = nullptr;
	Shader* shader = nullptr;
	Texture* texture = nullptr;
};