#include "Scene.h"

#include "Game/GameManager.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\fwd.hpp>
#include <glm\ext\matrix_transform.hpp>

#include "ResourceManager.h"
#include "Physics.h"
#include "Profiler.h"

#include <limits>
#include <chrono>

Scene* Scene::currentScene = nullptr;
bool Scene::inGameMode = false;


Scene::Scene()
{
    OnLoad();

    //Textures
    ResourceManager::LoadTexture("Terrain_Albedo.jpg");
    ResourceManager::LoadTexture("Building Area.jpg");
    ResourceManager::LoadTexture("Grocery.png");
    ResourceManager::LoadTexture("Building Area 1_Albedo.jpg");
    ResourceManager::LoadTexture("container.jpg");
    ResourceManager::LoadTexture("Stairs.jpg");
    ResourceManager::LoadTexture("Tree.png");
    //ResourceManager::LoadTexture("Gun_Cyberpunk Gun_BaseColor.jpg");
    ResourceManager::LoadTexture("Cube.jpg");
    ResourceManager::LoadTexture("gizmoSelectTexture.jpg");
    ResourceManager::LoadTexture("MagicaVoxelExports/abc.png");
    ResourceManager::LoadTexture("Chair/diffuse.png");
    ResourceManager::LoadTexture("House/Planks.png");

    //backpack
    //ResourceManager::LoadTexture("backpack/diffuse.jpg");
    //ResourceManager::LoadTexture("backpack/normal.png");
    //ResourceManager::LoadTexture("backpack/roughness.jpg");
    //ResourceManager::LoadTexture("backpack/specular.jpg");

    //Chair
    ResourceManager::LoadTexture("Chair/diffuse.png");

    ////HumanAnim
    ResourceManager::LoadTexture("Human/Human_Walk_1.png");
    //ResourceManager::LoadTexture("Human/Human_Walk_2.png");
    //ResourceManager::LoadTexture("Human/Human_Walk_3.png");
    //ResourceManager::LoadTexture("Human/Human_Walk_4.png");
    ResourceManager::LoadTexture("kenney_medievalrtspack/PNG/Retina/Tile/asd.png");
        
    //Models
    ResourceManager::LoadModel("Cube.obj");
    ResourceManager::LoadModel("Terrain.obj");
    ResourceManager::LoadModel("Buiilding Area.obj");
    ResourceManager::LoadModel("Grocery.obj");
    ResourceManager::LoadModel("rectangleOBJ.obj");
    ResourceManager::LoadModel("Human/Human.obj");
    ResourceManager::LoadModel("Stairs.obj");
   // ResourceManager::LoadModel("weapon.obj");
    ResourceManager::LoadModel("MagicaVoxelExports/abc.obj");
    ResourceManager::LoadModel("Chair/School Chair Offset.fbx");
    //ResourceManager::LoadModel("backpack/backpack.obj");
    //ResourceManager::LoadModel("House/House.fbx");

    //Shaders
    ResourceManager::LoadShader("StandardShader");


    camera = Camera(glm::vec3(0.0f, 0.0f, 20.0f), "StandardShader");
    GameManager gameManager = GameManager();

    //InstantiateModel(ResourceManager::GetModel("backpack/backpack.obj"));
   // InstantiateModel(ResourceManager::GetModel("House/House.fbx"));
    //InstantiateModel(ResourceManager::GetModel("Chair/School Chair Offset.fbx"));
    //InstantiateModel(ResourceManager::GetModel("Terrain.obj"));
}

Scene::~Scene()
{
}

void Scene::OnLoad()
{
    currentScene = this;
}

GameObject* Scene::Raycast()
{
    float x = (2.0f * mousePos.x) / winWidth - 1.0f;
    float y = 1.0f - (2.0f * mousePos.y) / winHeight;
    float z = 1.0f;
    glm::vec3 ray_nds = glm::vec3(x, y, z);

    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

    glm::vec3 ray_wor = (glm::inverse(view) * ray_eye);
    auto rayNorm = glm::normalize(ray_wor);

    GameObject* nearestObj = nullptr;
    float nearestDist = std::numeric_limits<float>::max();
    //  std::cout << "Ray: " << ray_wor.x << "x " << ray_wor.y << "y " << ray_wor.z << "z" << std::endl;
     // std::cout << "Nor: " << rayNorm.x << "x " << rayNorm.y << "y " << rayNorm.z << "z" << std::endl;

    for (std::unique_ptr<GameObject>& targetObj : objects)
    {
        SpriteComponent* objSpriteComponent = targetObj->GetComponent<SpriteComponent>();
        Mesh* meshNewFromComponent = nullptr;
        if (objSpriteComponent != nullptr)
        {
            meshNewFromComponent = objSpriteComponent->GetMeshNew();
        }
        if (meshNewFromComponent == nullptr)
            continue;

        glm::vec3 camPos = camera.GetLocalPosition();
        glm::vec3 targetObjPos = targetObj->transform.GetGlobalPosition();
        //  std::cout << "CamPos: " << camPos.x << "x " << camPos.y << "y " << camPos.z << "z" << std::endl;
        //  std::cout << "targetObjPos: " << targetObjPos.x << "x " << targetObjPos.y << "y " << targetObjPos.z << "z" << std::endl;
        //  std::cout << "DistZ: " << glm::abs(camPos.z - targetObjPos.z) << "z" << std::endl;

        float distanceToObj = glm::distance(camPos, targetObjPos);
        float distanceRayToObj = glm::distance(rayNorm * distanceToObj + camPos, targetObjPos);
        // std::cout << "DistToObj: " << distanceToObj << std::endl;
       //  std::cout << "Dist Ray: " << distanceRayToObj << std::endl;

        glm::vec3 rayToTargetObjVec = rayNorm * distanceToObj + camPos;
        //std::cout << "rayHit: " << rayToTargetObjVec.x << "x " << rayToTargetObjVec.y << "y " << rayToTargetObjVec.z << "z" << std::endl;

        //rayToTargetObjVec -= targetObjPos;
        //if (glm::abs(rayToTargetObjVec.x) < 2 && glm::abs(rayToTargetObjVec.y) < 2 && glm::abs(rayToTargetObjVec.z) < 2)

        Shader* objShader = objSpriteComponent->GetShader();
        if (meshNewFromComponent->GetBounds().Intersects(rayToTargetObjVec, targetObjPos))
        {
            //std::cout << "\tYes! Ray is hitting object!\n";

      /*      if(objShader != nullptr)
                objShader->SetVec4("_FragColor", 0.0f, 0.2f, 0.0f, 0.0f);*/

            if (distanceRayToObj < nearestDist)
            {
                nearestDist = distanceRayToObj;
                nearestObj = targetObj.get();
            }
        }
        else
        {
            //std::cout << "\No! Ray is not hitting object!\n";
            //if (objShader != nullptr)
            //    objShader->SetVec4("_FragColor", 0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    return nearestObj;
}

const std::vector<std::unique_ptr<GameObject>>* const Scene::GetObjectsPtr() const
{
    return &objects;
}


const bool& Scene::IsInGameMode()
{
    return inGameMode;
}

void Scene::EnterGameMode()
{
    if (inGameMode)
        return;

    std::cout<<"\tENTER::GAME_MODE"<<std::endl;

    inGameMode = true;
    objectsCopy.clear();
    for (const std::unique_ptr<GameObject>& obj : objects)
    {
        if (obj->transform.GetParent() == nullptr)
        {
            std::unique_ptr<GameObject> newObj = std::make_unique<GameObject>(*obj.get());
            objectsCopyActiveData.push_back(newObj->IsActive());
            newObj->SetActive(false);

            DuplicateChildsCopy(obj.get(), newObj.get());

            objectsCopy.push_back(std::move(newObj));
        }

    }
}

void Scene::ExitGameMode()
{
    if (!inGameMode)
        return;

    std::cout << "\tENTER::EDITOR_MODE" << std::endl;



    inGameMode = false;
    for (std::unique_ptr<GameObject>& obj : objects)
    {
        Destroy(obj.get());
    }   

    size_t i = 0;
    for (const std::unique_ptr<GameObject>& obj : objectsCopy)
    {
        if (obj->transform.GetParent() == nullptr)
        {
            DuplicateGameObject(obj.get());
            //obj->SetActive(objectsCopyActiveData[i]);
            obj->SetActive(true);
        }
        i++;
    }
    objectsCopy.clear();
    objectsCopyActiveData.clear();
}

void Scene::Update(const float& deltaTime, Shader& camShader, glm::mat4 _projection, glm::mat4 _view)
{
    //Update
    std::string sampleName = "Update time";
    Profiler::BeginSample(sampleName);

    projection = _projection;
    view = _view;
    //Update GameObjects
    if (inGameMode)
    {
        //Update
        for (std::unique_ptr<GameObject>& obj : objects)
        {
            if (!obj->IsActive())
                continue;

            obj->Update(deltaTime);
        }

        Physics::Perform();
        
        //LateUpdate
        for (std::unique_ptr<GameObject>& obj : objects)
        {
            if (!obj->IsActive())
                continue;

            obj->LateUpdate(deltaTime);
        }
    }

    DestroyPostponed();
    Profiler::EndSample();

    //Draw
    sampleName = "Draw time";
    Profiler::BeginSample(sampleName);
    
    Renderer::DrawNew(camShader);

    //Draw gizmos
    if (Editor::CanDrawGizmos())
    {
        for (std::unique_ptr<GameObject>& obj : objects)
        {
            if (!obj->IsActive())
                continue;

            obj->ShowOnGizmos();
        }
    }

    Profiler::EndSample();
}

Camera& Scene::GetCamera()
{
    return camera;
}

Scene& Scene::GetCurrentScene()
{
    return *currentScene;
}

void Scene::InstantiateModel(const Model const* model)
{
    for (const Mesh& mesh : model->meshes)
    {
        GameObject* obj = Instantiate<GameObject>(glm::vec3(0, 0, 0));
        SpriteComponent* spriteComp = obj->AddComponent<SpriteComponent>();
        std::string meshName = model->GetShortDirectory() + "->" + mesh.name;
        spriteComp->SetMeshNew(meshName.c_str());
        spriteComp->SetShader("StandardShader");
        spriteComp->SetTexture("Cube.jpg");
        obj->name = mesh.name;
    }
}

GameObject* Scene::DuplicateGameObject(const GameObject* obj)
{
    glm::vec3 originalPos = obj->transform.GetLocalPosition();
    std::unique_ptr<GameObject> newObj = std::make_unique<GameObject>(*obj);
    objects.push_back(std::move(newObj));
    GameObject* newObjPtr = objects.back().get();
    std::cout << "[Copy GameObject] Object size: " << objects.size() << "\n";
    newObjPtr->SetActive(true);

    //Duplicate all childs
    for (const GameObject* child : obj->transform.GetChilds())
    {
        DuplicateChilds(child, newObjPtr);
    }

    return newObjPtr;
}

void Scene::DuplicateChilds(const GameObject* obj, GameObject* parent)
{
    std::unique_ptr<GameObject> newObj = std::make_unique<GameObject>(*obj);
    objects.push_back(std::move(newObj));
    GameObject* newObjPtr = objects.back().get();
    newObjPtr->transform.SetParent(parent);
    newObjPtr->SetActive(true);

    for (const GameObject* child : obj->transform.GetChilds())
    {
        DuplicateChilds(child, newObjPtr);
    }
}

void Scene::DuplicateChildsCopy(GameObject* obj, GameObject* parent)
{
    for (GameObject* child : obj->transform.GetChilds())
    {
        std::unique_ptr<GameObject> newChild = std::make_unique<GameObject>(*child);
        objectsCopyActiveData.push_back(newChild->IsActive());
        newChild->SetActive(false);
        newChild.get()->transform.SetParent(parent);
        DuplicateChildsCopy(child, newChild.get());
        objectsCopy.push_back(std::move(newChild));
    }
}

void Scene::Destroy(GameObject* obj)
{
    objectsToDestroy.push_back(obj);
    //Destroy all obj childs
    for (size_t i = 0; i < obj->transform.GetChilds().size(); i++)
    {
        Destroy(obj->transform.GetChilds()[i]);
    }
}

void Scene::DestroyPostponed()
{
    //Remove objects from end to begin (to remove GameObject childs first)
    for (int i = objectsToDestroy.size()-1; i >= 0 ; i--)
    {
        GameObject* targetObj = objectsToDestroy[i];
        std::vector<std::unique_ptr<GameObject>>::iterator it = std::find_if(objects.begin(), objects.end(), [&targetObj](std::unique_ptr<GameObject>& element) { return element.get() == targetObj; });
        if (it != objects.end())
        {
            it->reset(nullptr);
            objects.erase(it);
            Editor::Select(nullptr);
        }
    }
    if (objectsToDestroy.size() > 0)
        std::cout << "[DESTROY_POSTPONED] Object size: " << objects.size() << "\n";

    objectsToDestroy.clear();
}

void Scene::SetMousePos(const glm::vec2& pos, int w, int h)
{
    mousePos = pos;
    winWidth = w;
    winHeight = h;
}

glm::vec2 Scene::GetMousePos() const
{
    return mousePos;
}
