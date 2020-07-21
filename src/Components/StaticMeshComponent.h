#pragma once
#include "Component.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../Mesh.h"


class StaticMeshComponent : public Component
{
public:
	StaticMeshComponent(GameObject* _parent);
	~StaticMeshComponent();

	StaticMeshComponent(const StaticMeshComponent& comp);
	StaticMeshComponent& operator=(const StaticMeshComponent& comp) = delete;

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

	void SetSpecularTexture(const char* texPath);
	void SetSpecularTexture(Texture* tex);
	const Texture* GetSpecularTexture();

	glm::vec4 color;
	glm::vec3 specular{0.5, 0.5f, 0.5f};
	float shininess = 32;

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

private:
	Mesh* meshNew = nullptr;
	Shader* shader = nullptr;
	Texture* texture = nullptr;
	Texture* specularTexture = nullptr;
};