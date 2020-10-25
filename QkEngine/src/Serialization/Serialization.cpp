#include "Serialization.h"
#include <iostream>
#include "../Scene.h"
#include "../Components/Lighting/DirectionalLightComponent.h"
#include "../Components/Lighting/PointLightComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/AI/NavMeshAgent.h"

void Serializer::Serialize()
{
    // 1. Parse a JSON string into DOM.
    Document d(kObjectType);
    Document::AllocatorType& allocator = d.GetAllocator();
    
    Value array(kArrayType);
    for (const std::unique_ptr<GameObject>& obj : Scene::GetCurrentScene().objects)
    {
        Value object(kObjectType);

        object.AddMember("Name", GenericStringRef(obj->name.c_str()), allocator);

        //Transform
        glm::vec3 pos = obj->transform.GetLocalPosition();
        glm::quat rot = obj->transform.GetLocalRotation();
        glm::vec3 scale = obj->transform.GetLocalScale();
        SerializeVec3("Position", pos, object, allocator);
        SerializeVec3("Rotation", glm::vec3(rot.x, rot.y, rot.z), object, allocator);
        SerializeVec3("Scale", scale, object, allocator);

        //Components
        Value components(kArrayType);
        const std::vector<std::unique_ptr<Component>>* comps = obj->GetAllComponents();
        for (size_t i = 0; i < comps->size(); i++)
        {
            Value savingComponent(kObjectType);
            Component* currComp = (*comps)[i].get();
            const char* compName = currComp->name.c_str();
            savingComponent.AddMember("Type", GenericStringRef(compName), allocator);

            if (StaticMeshComponent* staticMeshComp = dynamic_cast<StaticMeshComponent*>(currComp))
            {
                const char* meshName = staticMeshComp->GetMeshNew() != nullptr ? staticMeshComp->GetMeshNew()->name.c_str() : "";
                savingComponent.AddMember("Mesh", GenericStringRef(meshName), allocator);

                const char* textureName = staticMeshComp->GetTexture() != nullptr ? staticMeshComp->GetTexture()->name.c_str() : "";
                savingComponent.AddMember("Texture", GenericStringRef(textureName), allocator);

                const char* specularTextureName = staticMeshComp->GetSpecularTexture() != nullptr ? staticMeshComp->GetSpecularTexture()->name.c_str() : "";
                savingComponent.AddMember("SpecularTexture", GenericStringRef(specularTextureName), allocator);

                const char* shaderName = staticMeshComp->GetShader() != nullptr ? staticMeshComp->GetShader()->name.c_str() : "";
                savingComponent.AddMember("Shader", GenericStringRef(shaderName), allocator);

                SerializeVec4("Color", staticMeshComp->color, savingComponent, allocator);
                SerializeVec3("Specular", staticMeshComp->specular, savingComponent, allocator);
                savingComponent.AddMember("Shininess", staticMeshComp->shininess, allocator);
            }
            else if (DirectionalLightComponent* directionalLightComp = dynamic_cast<DirectionalLightComponent*>(currComp))
            {
                SerializeVec3("Color", directionalLightComp->GetColor(), savingComponent, allocator);
            }
            else if (PointLightComponent* pointLightComponent = dynamic_cast<PointLightComponent*>(currComp))
            {
                SerializeVec3("Color", pointLightComponent->GetColor(), savingComponent, allocator);
                savingComponent.AddMember("Intensity", pointLightComponent->intensity, allocator);
            }
            else if (BoxColliderComponent* boxCollider = dynamic_cast<BoxColliderComponent*>(currComp))
            {
                savingComponent.AddMember("DynamicObstacle", boxCollider->isDynamicObstacle, allocator);
                savingComponent.AddMember("IgnoreNavigation", boxCollider->ignoreNavigation, allocator);
                savingComponent.AddMember("Pushable", boxCollider->isPushable, allocator);
                savingComponent.AddMember("Trigger", boxCollider->isTrigger, allocator);
                SerializeVec3("Center", boxCollider->GetCenter(), savingComponent, allocator);
                SerializeVec3("Extents", boxCollider->GetExtents(), savingComponent, allocator);
            }
            else if (PlayerComponent* playerComponent = dynamic_cast<PlayerComponent*>(currComp))
            {
                savingComponent.AddMember("MoveSpeed", playerComponent->moveSpeed, allocator);
            }
            else if (NavMeshAgentComponent* navAgentComponent = dynamic_cast<NavMeshAgentComponent*>(currComp))
            {
                savingComponent.AddMember("MoveSpeed", navAgentComponent->movementSpeed, allocator);
            }

            components.PushBack(savingComponent, allocator);
        }

        object.AddMember("Components", components, allocator);
        array.PushBack(object, allocator);
    }

    d.AddMember("Objects", array, allocator);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;

    std::fstream file("Project.json", std::ios::out | std::ios::trunc);
    if (file.is_open())
    {
        std::cout << "Saving!\n";
        file << buffer.GetString();
        file.close();
    }
}

void Serializer::Deserialize()
{
    std::ifstream ifs("Project.json", std::ios::in);
    if (ifs.is_open())
    {
        std::cout << "Loading\n";
        for (const std::unique_ptr<GameObject>& obj : Scene::GetCurrentScene().objects)
        {
            Scene::GetCurrentScene().Destroy(obj.get());
        }

        std::ostringstream ss;
        ss << ifs.rdbuf(); // reading data

        Document d;
        d.Parse(ss.str().c_str());

        if (d.HasMember("Objects"))
        {
            if (d["Objects"].IsArray())
            {
                for (Value::ConstValueIterator itr = d["Objects"].Begin(); itr != d["Objects"].End(); ++itr)
                {
                    GameObject* instance = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0,0,0));
                    auto gameObj = itr->GetObject();

                    instance->name = itr->GetObject()["Name"].GetString();

                    //Transform
                    instance->transform.SetLocalPosition(DeserializeVec3(gameObj["Position"].GetObject()));
                    glm::vec3 rot = DeserializeVec3(gameObj["Rotation"].GetObject());
                    instance->transform.SetLocalRotation(glm::quat(1, rot.x, rot.y, rot.z));
                    instance->transform.SetLocalScale(DeserializeVec3(gameObj["Scale"].GetObject()));

                    //Components
                    auto begin = itr->GetObject()["Components"].Begin();
                    auto end = itr->GetObject()["Components"].End();
                    std::cout << "count: " << end - begin << std::endl;

                    for (Value::ConstValueIterator itrComp = begin; itrComp != end; ++itrComp)
                    {
                        auto typeName = itrComp->GetObject()["Type"].GetString();
                        std::cout << "typeName: " << typeName << std::endl;
                        if (strcmp(typeName, "StaticMeshComponent") == 0)
                        {
                            StaticMeshComponent* meshComp = instance->AddComponent<StaticMeshComponent>();
                            auto meshName = itrComp->GetObject()["Mesh"].GetString();
                            meshComp->SetMeshNew(meshName);
                            auto textureName = itrComp->GetObject()["Texture"].GetString();
                            meshComp->SetTexture(textureName);
                            auto specularTextureName = itrComp->GetObject()["SpecularTexture"].GetString();
                            meshComp->SetSpecularTexture(specularTextureName);
                            auto shaderName = itrComp->GetObject()["Shader"].GetString();
                            meshComp->SetShader(shaderName);

                            meshComp->color = DeserializeVec4(itrComp->GetObject()["Color"].GetObject());
                            meshComp->specular = DeserializeVec3(itrComp->GetObject()["Specular"].GetObject());
                            meshComp->shininess = itrComp->GetObject()["Shininess"].GetDouble();
                        }
                        else if (strcmp(typeName, "DirectionalLightComponent") == 0)
                        {
                            DirectionalLightComponent* dirLightComp = instance->AddComponent<DirectionalLightComponent>();
                            dirLightComp->SetColor(DeserializeVec3(itrComp->GetObject()["Color"].GetObject()));
                        }
                        else if (strcmp(typeName, "PointLightComponent") == 0)
                        {
                            PointLightComponent* pointLightComp = instance->AddComponent<PointLightComponent>();
                            pointLightComp->lightColor = DeserializeVec3(itrComp->GetObject()["Color"].GetObject());
                            pointLightComp->intensity = itrComp->GetObject()["Intensity"].GetDouble();
                        }
                        else if (strcmp(typeName, "BoxColliderComponent") == 0)
                        {
                            BoxColliderComponent* boxCollComp = instance->AddComponent<BoxColliderComponent>();
                            boxCollComp->isDynamicObstacle = itrComp->GetObject()["DynamicObstacle"].GetBool();
                            boxCollComp->ignoreNavigation = itrComp->GetObject()["IgnoreNavigation"].GetBool();
                            boxCollComp->isPushable = itrComp->GetObject()["Pushable"].GetBool();
                            boxCollComp->isTrigger = itrComp->GetObject()["Trigger"].GetBool();
                            boxCollComp->center = DeserializeVec3(itrComp->GetObject()["Center"].GetObject());
                            boxCollComp->bounds.SetExtents(DeserializeVec3(itrComp->GetObject()["Extents"].GetObject()));
                        }
                        else if (strcmp(typeName, "PlayerComponent") == 0)
                        {
                            PlayerComponent* playerComp = instance->AddComponent<PlayerComponent>();
                            playerComp->moveSpeed = itrComp->GetObject()["MoveSpeed"].GetDouble();
                        }
                        else if (strcmp(typeName, "NavMeshAgentComponent") == 0)
                        {
                            NavMeshAgentComponent* navAgentComp = instance->AddComponent<NavMeshAgentComponent>();
                            navAgentComp->movementSpeed = itrComp->GetObject()["MoveSpeed"].GetDouble();
                        }
                    }
                }
            }
        }
    }
}

void Serializer::SerializeVec3(const char* name, const glm::vec3& vec, Value& val, Document::AllocatorType& allocator)
{
    Value newObj(kObjectType);
    newObj.AddMember("x", vec.x, allocator);
    newObj.AddMember("y", vec.y, allocator);
    newObj.AddMember("z", vec.z, allocator);
    val.AddMember(GenericStringRef(name), newObj, allocator);
}

glm::vec3 Serializer::DeserializeVec3(const GenericObject<true, Value>& obj)
{
    return glm::vec3(obj["x"].GetDouble(), obj["y"].GetDouble(), obj["z"].GetDouble());
}

void Serializer::SerializeVec4(const char* name, const glm::vec4& vec, Value& saveTo, Document::AllocatorType& allocator)
{
    Value newObj(kObjectType);
    newObj.AddMember("x", vec.x, allocator);
    newObj.AddMember("y", vec.y, allocator);
    newObj.AddMember("z", vec.z, allocator);
    newObj.AddMember("w", vec.w, allocator);
    saveTo.AddMember(GenericStringRef(name), newObj, allocator);
}

glm::vec4 Serializer::DeserializeVec4(const GenericObject<true, Value>& obj)
{
    return glm::vec4(obj["x"].GetDouble(), obj["y"].GetDouble(), obj["z"].GetDouble(), obj["w"].GetDouble());
}
