#include "Serialization.h"
#include <iostream>
#include <algorithm>
#include "../Scene.h"
#include "../Components/Lighting/DirectionalLightComponent.h"
#include "../Components/Lighting/PointLightComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/AI/NavMeshAgent.h"
#include "../Lighting.h"
#include "../Navigation/NavMesh.h"
#include "../Window.h"

void Serializer::Serialize(std::string fileName)
{
    // 1. Parse a JSON string into DOM.
    Document d(kObjectType);
    Document::AllocatorType& allocator = d.GetAllocator();
    
    Value array(kArrayType);
    for (const std::unique_ptr<GameObject>& obj : Scene::GetCurrentScene().objects)
    {
        Value object(kObjectType);

        object.AddMember("Name", GenericStringRef(obj->name.c_str()), allocator);
        object.AddMember("Active", obj->IsActive(), allocator);

        //Transform
        glm::vec3 pos = obj->transform.GetLocalPosition();
        glm::quat rot = obj->transform.GetLocalRotation();
        glm::vec3 scale = obj->transform.GetLocalScale();
        SerializeVec3("Position", pos, object, allocator);
        SerializeVec3("Rotation", glm::vec3(rot.x, rot.y, rot.z), object, allocator);
        SerializeVec3("Scale", scale, object, allocator);
        //Save parent
        int parentIndex = -1;
        for (size_t i = 0; i < Scene::GetCurrentScene().objects.size(); i++)
        {
            GameObject* par = obj.get()->transform.GetParent();
            if (par == nullptr)
                break;
            if (Scene::GetCurrentScene().objects[i].get() == par)
                parentIndex = i;
        }
        object.AddMember("Parent", parentIndex, allocator);

        //Components
        Value components(kArrayType);
        const std::vector<std::unique_ptr<Component>>* comps = obj->GetAllComponents();
        for (size_t i = 0; i < comps->size(); i++)
        {
            Value savingComponent(kObjectType);
            Component* currComp = (*comps)[i].get();
            const char* compName = currComp->name.c_str();
            savingComponent.AddMember("Type", GenericStringRef(compName), allocator);

            if (strcmp(compName, "StaticMeshComponent") == 0)
            {             
                StaticMeshComponent* staticMeshComp = dynamic_cast<StaticMeshComponent*>(currComp);
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
            else if (strcmp(compName, "AnimatedSpriteComponent") == 0)
            {
                AnimatedSpriteComponent* animatedSpriteComp = dynamic_cast<AnimatedSpriteComponent*>(currComp);
                const char* meshName = animatedSpriteComp->GetMeshNew() != nullptr ? animatedSpriteComp->GetMeshNew()->name.c_str() : "";
                savingComponent.AddMember("Mesh", GenericStringRef(meshName), allocator);

                const char* textureName = animatedSpriteComp->GetTexture() != nullptr ? animatedSpriteComp->GetTexture()->name.c_str() : "";
                savingComponent.AddMember("Texture", GenericStringRef(textureName), allocator);

                const char* specularTextureName = animatedSpriteComp->GetSpecularTexture() != nullptr ? animatedSpriteComp->GetSpecularTexture()->name.c_str() : "";
                savingComponent.AddMember("SpecularTexture", GenericStringRef(specularTextureName), allocator);

                const char* shaderName = animatedSpriteComp->GetShader() != nullptr ? animatedSpriteComp->GetShader()->name.c_str() : "";
                savingComponent.AddMember("Shader", GenericStringRef(shaderName), allocator);

                const char* animationName = animatedSpriteComp->currentAnimation != nullptr ? animatedSpriteComp->currentAnimation->name.c_str() : "";
                std::cout << std::endl << "Saved Anim: " << animationName << std::endl << std::endl;

                savingComponent.AddMember("Animation", GenericStringRef(animationName), allocator);

                SerializeVec4("Color", animatedSpriteComp->color, savingComponent, allocator);
                SerializeVec3("Specular", animatedSpriteComp->specular, savingComponent, allocator);
                savingComponent.AddMember("Shininess", animatedSpriteComp->shininess, allocator);
                savingComponent.AddMember("FrameTime", animatedSpriteComp->frameTime, allocator);
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
            else if (MoveComponent* moveComponent = dynamic_cast<MoveComponent*>(currComp))
            {
                SerializeVec3("MoveVec", moveComponent->moveVec, savingComponent, allocator);
            }
            components.PushBack(savingComponent, allocator);
        }

        object.AddMember("Components", components, allocator);
        array.PushBack(object, allocator);
    }

    d.AddMember("Objects", array, allocator);

    //Camera
    Value cameraObj(kObjectType);
    SerializeVec4("ClearColor", Scene::GetCurrentScene().GetCamera().GetClearColor(), cameraObj, allocator);
    d.AddMember("Camera", cameraObj, allocator);

    //Lighting
    Value lightingObj(kObjectType);
    SerializeVec3("AmbientColor", Lighting::ambientLightColor, lightingObj, allocator);
    SerializeVec3("FogColor", Lighting::fogColor, lightingObj, allocator);
    lightingObj.AddMember("FogDensity", Lighting::fogDensity, allocator);
    d.AddMember("Lighting", lightingObj, allocator);

    //Navigation
    Value navigationObj(kObjectType);
    SerializeVec3("StartPos", NavMesh::startPos, navigationObj, allocator);
    navigationObj.AddMember("Width", NavMesh::width, allocator);
    navigationObj.AddMember("NodeSize", NavMesh::nodeSize, allocator);

    Value navigationNodesArr(kArrayType);
    for (const NavMeshNode& node : NavMesh::nodes)
    {
        Value nodeObj(kObjectType);
        SerializeVec3("Position", node.position, nodeObj, allocator);
        nodeObj.AddMember("Colliding", node.isColliding, allocator);
        nodeObj.AddMember("GridX", node.gridX, allocator);
        nodeObj.AddMember("GridY", node.gridY, allocator);
        navigationNodesArr.PushBack(nodeObj, allocator);
    }
    navigationObj.AddMember("Nodes", navigationNodesArr, allocator);

    d.AddMember("Navigation", navigationObj, allocator);


    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    d.Accept(writer);

    std::cout << buffer.GetString() << std::endl;

    std::fstream file(fileName + ".json", std::ios::out | std::ios::trunc);
    if (file.is_open())
    {
        std::cout << "Saving!\n";
        file << buffer.GetString();
        file.close();
    }
}

void Serializer::Deserialize(std::string fileName)
{
    std::ifstream ifs(fileName + ".json", std::ios::in);
    if (ifs.is_open())
    {
        Scene::GetCurrentScene().ClearWidgets();

        Window::GetCurrentWindow()->ResetDeltaTime();
        std::cout << "Loading\n";
        for (const std::unique_ptr<GameObject>& obj : Scene::GetCurrentScene().objects)
        {
            Scene::GetCurrentScene().Destroy(obj.get());
        }
        Scene::GetCurrentScene().DestroyPostponed();

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
                    instance->isActive = itr->GetObject()["Active"].GetBool();

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
                        else if (strcmp(typeName, "AnimatedSpriteComponent") == 0)
                        {
                            AnimatedSpriteComponent* animatedSpriteComp = instance->AddComponent<AnimatedSpriteComponent>();
                            auto meshName = itrComp->GetObject()["Mesh"].GetString();
                            animatedSpriteComp->SetMeshNew(meshName);
                            auto textureName = itrComp->GetObject()["Texture"].GetString();
                            animatedSpriteComp->SetTexture(textureName);
                            auto specularTextureName = itrComp->GetObject()["SpecularTexture"].GetString();
                            animatedSpriteComp->SetSpecularTexture(specularTextureName);
                            auto shaderName = itrComp->GetObject()["Shader"].GetString();
                            animatedSpriteComp->SetShader(shaderName);
                            auto animationName = itrComp->GetObject()["Animation"].GetString();
                            std::cout << "Anim: " << animationName << std::endl;

                            animatedSpriteComp->SetCurrentAnimation(ResourceManager::GetSpriteAnimation(animationName));

                            animatedSpriteComp->color = DeserializeVec4(itrComp->GetObject()["Color"].GetObject());
                            animatedSpriteComp->specular = DeserializeVec3(itrComp->GetObject()["Specular"].GetObject());
                            animatedSpriteComp->shininess = itrComp->GetObject()["Shininess"].GetDouble();
                            animatedSpriteComp->frameTime = itrComp->GetObject()["FrameTime"].GetDouble();
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
                        else if (strcmp(typeName, "MoveComponent") == 0)
                        {
                            MoveComponent* moveComp = instance->AddComponent<MoveComponent>();
                            moveComp->moveVec = DeserializeVec3(itrComp->GetObject()["MoveVec"].GetObject());
                        }
                    }
                }
            }
        }

        //Parenting
        if (d.HasMember("Objects"))
        {
            if (d["Objects"].IsArray())
            {
                int i = 0;
                for (Value::ConstValueIterator itr = d["Objects"].Begin(); itr != d["Objects"].End(); ++itr)
                {
                    auto gameObj = itr->GetObject();
                    int parentIndex = gameObj["Parent"].GetInt();
                    if (parentIndex >= 0)
                    {
                        Scene::GetCurrentScene().objects[i]->transform.SetParent(Scene::GetCurrentScene().objects[parentIndex].get());
                    }
                    i++;
                }
            }
        }

        //Camera
        auto camera = d["Camera"].GetObject();
        Scene::GetCurrentScene().GetCamera().SetClearColor(DeserializeVec4(camera["ClearColor"].GetObject()));

        //Lighting
        auto lighting = d["Lighting"].GetObject();
        Lighting::ambientLightColor = DeserializeVec3(lighting["AmbientColor"].GetObject());
        Lighting::fogColor = DeserializeVec3(lighting["FogColor"].GetObject());
        Lighting::fogDensity = lighting["FogDensity"].GetDouble();

        //Navigation
        auto navigation = d["Navigation"].GetObject();
        NavMesh::startPos = DeserializeVec3(navigation["StartPos"].GetObject());
        NavMesh::width = navigation["Width"].GetInt();
        NavMesh::nodeSize = navigation["NodeSize"].GetDouble();
        NavMesh::nodes.clear();
        for (Value::ConstValueIterator itr = navigation["Nodes"].Begin(); itr != navigation["Nodes"].End(); ++itr)
        {
            NavMeshNode node = NavMeshNode(
                DeserializeVec3(itr->GetObject()["Position"].GetObject())
                , itr->GetObject()["GridX"].GetInt()
                , itr->GetObject()["GridY"].GetInt());

            node.isColliding = itr->GetObject()["Colliding"].GetBool();

            NavMesh::nodes.push_back( node );
        }


        //Invoke Start() for all object on loaded scene
        Scene::GetCurrentScene().InvokeStart();
    }
    else
    {
        std::cout << "Can't find scene file: " << fileName << "\n";
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

glm::vec3 Serializer::DeserializeVec3(const GenericObject<false, Value>& obj)
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

glm::vec4 Serializer::DeserializeVec4(const GenericObject<false, Value>& obj)
{
    return glm::vec4(obj["x"].GetDouble(), obj["y"].GetDouble(), obj["z"].GetDouble(), obj["w"].GetDouble());
}
