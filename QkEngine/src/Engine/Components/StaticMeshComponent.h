#pragma once
#include "Component.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../Mesh.h"


class StaticMeshComponent : public Component
{
public:
	StaticMeshComponent(GameObject* _parent);
	virtual ~StaticMeshComponent();

	StaticMeshComponent(const StaticMeshComponent& comp);
	StaticMeshComponent& operator=(const StaticMeshComponent& comp) = delete;

	void Update(const float& deltaTime) override;
	void LateUpdate(const float& deltaTime) override;

	void ShowOnGizmos() override;
	void ShowOnInspector() override;
	
	void SetMesh(const char* meshPath);
	Mesh* GetMesh();

	void SetShader(Shader* _shader);
	void SetShader(const char* shaderPath);
	Shader* GetShader();

	void SetTexture(const char* texPath);
	void SetTexture(Texture* tex);
	Texture* GetTexture();

	void SetSpecularTexture(const char* texPath);
	void SetSpecularTexture(Texture* tex);
	Texture* GetSpecularTexture();

	Bounds GetBounds() const;

	glm::vec4 color;
	glm::vec3 specular{0.5, 0.5f, 0.5f};
	float shininess = 32;

	std::unique_ptr<Component> MakeCopy(GameObject* newParent) const override;

private:
	Mesh* mesh = nullptr;
	Shader* shader = nullptr;
	Texture* texture = nullptr;
	Texture* specularTexture = nullptr;
};