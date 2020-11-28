#include "Scene.h"

#include "../Game/GameManager.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\fwd.hpp>
#include <glm\ext\matrix_transform.hpp>

#include "ResourceManager.h"
#include "Physics/Physics.h"
#include "Physics/Ray.h"
#include "Profiler.h"
#include "Window.h"
#include "Gizmos.h"
#include "Navigation/NavMesh.h"
#include "InputManager.h"

#include <limits>
#include <chrono>

Scene* Scene::currentScene = nullptr;
bool Scene::inGameMode = false;


Scene::Scene()
{
    OnLoad();

    //Textures
    //Editor textures
    ResourceManager::LoadTexture("Editor/PlayIcon.png");
    ResourceManager::LoadTexture("Editor/StopIcon.png");
    ResourceManager::LoadTexture("gizmoSelectTexture.jpg");
    
    ResourceManager::LoadTexture("Grey.png");
    ResourceManager::LoadTexture("Dark.png");


    //backpack
    //ResourceManager::LoadTexture("backpack/diffuse.jpg");
    //ResourceManager::LoadTexture("backpack/normal.png");
    //ResourceManager::LoadTexture("backpack/roughness.jpg");
    //ResourceManager::LoadTexture("backpack/specular.jpg");

    //Animation textures
    //Idle
    ResourceManager::LoadTexture("Adventurer/adventurer-idle-00.png");
    ResourceManager::LoadTexture("Adventurer/adventurer-idle-01.png");
    //Run
    ResourceManager::LoadTexture("Adventurer/adventurer-run-00.png");
    ResourceManager::LoadTexture("Adventurer/adventurer-run-01.png");
    ResourceManager::LoadTexture("Adventurer/adventurer-run-02.png");
    ResourceManager::LoadTexture("Adventurer/adventurer-run-03.png");
    ResourceManager::LoadTexture("Adventurer/adventurer-run-04.png");
    ResourceManager::LoadTexture("Adventurer/adventurer-run-05.png");
    //Jump
    ResourceManager::LoadTexture("Adventurer/adventurer-jump-03.png");

    //SpriteAnimations
    //Idle
    std::vector<Texture*> idle;
    Texture* frame0 = ResourceManager::GetTexture("Adventurer/adventurer-idle-00.png");
    idle.push_back(frame0);
    Texture* frame1 = ResourceManager::GetTexture("Adventurer/adventurer-idle-01.png");
    idle.push_back(frame1);
    ResourceManager::LoadSpriteAnimation(std::move(idle), "Idle");
    //Run
    std::vector<Texture*> run;
    run.push_back(ResourceManager::GetTexture("Adventurer/adventurer-run-00.png"));
    run.push_back(ResourceManager::GetTexture("Adventurer/adventurer-run-01.png"));
    run.push_back(ResourceManager::GetTexture("Adventurer/adventurer-run-02.png"));
    run.push_back(ResourceManager::GetTexture("Adventurer/adventurer-run-03.png"));
    run.push_back(ResourceManager::GetTexture("Adventurer/adventurer-run-04.png"));
    run.push_back(ResourceManager::GetTexture("Adventurer/adventurer-run-05.png"));
    ResourceManager::LoadSpriteAnimation(std::move(run), "Run");
    //Jump
    std::vector<Texture*> jump;
    ResourceManager::LoadModel("Human/Human.obj");
    ResourceManager::LoadModel("Vegetation/Tree.fbx");
    jump.push_back(ResourceManager::GetTexture("Adventurer/adventurer-jump-03.png"));
    ResourceManager::LoadSpriteAnimation(std::move(jump), "Jump");

    //Models
    ResourceManager::LoadModel("Arrow.fbx");
    ResourceManager::LoadModel("Cube.obj");

    //Shaders
    ResourceManager::LoadShader("StandardShader");
    ResourceManager::LoadShader("UnlitShader");
    ResourceManager::LoadShader("UIShader");

    //Audio
    ResourceManager::LoadAudioClip("Jump.wav");

    camera = Camera(glm::vec3(0.0f, 0.0f, 20.0f));
    GameManager gameManager = GameManager();

    //InstantiateModel(ResourceManager::GetModel("backpack/backpack.obj"));
   // InstantiateModel(ResourceManager::GetModel("House/House.fbx"));
    //InstantiateModel(ResourceManager::GetModel("Chair/School Chair Offset.fbx"));
    //InstantiateModel(ResourceManager::GetModel("Terrain.obj"));

    Renderer::Init();
}

Scene::~Scene()
{
}

void Scene::OnLoad()
{
    currentScene = this;
}
bool intersect(const Ray& r, Bounds bounds, glm::vec3 pos)
{
    glm::vec3 min = -bounds.Extents() + pos;
    glm::vec3 max = bounds.Extents() + pos;

    glm::vec3 origin = r.GetOrigin();
    glm::vec3 direction = r.GetDirection();

    float tmin = (min.x - origin.x) / direction.x;
    float tmax = (max.x - origin.x) / direction.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (min.y - origin.y) / direction.y;
    float tymax = (max.y - origin.y) / direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (min.z - r.GetOrigin().z) / r.GetDirection().z;
    float tzmax = (max.z - r.GetOrigin().z) / r.GetDirection().z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    return true;
}


GameObject* Scene::Raycast()
{
    glm::vec2 glfwWinPos = Window::GetCurrentWindow()->GetGLFWWindowPosition();
    glm::vec2 viewportPos = Editor::GetViewportPosition();
    mousePos -= viewportPos;
    mousePos += glfwWinPos;
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
        StaticMeshComponent* objStaticMeshComponent = targetObj->GetComponent<StaticMeshComponent>();
        Mesh* meshNewFromComponent = nullptr;
        if (objStaticMeshComponent != nullptr)
        {
            meshNewFromComponent = objStaticMeshComponent->GetMeshNew();
        }
        if (meshNewFromComponent == nullptr)
            continue;

        glm::vec3 camPos = camera.GetLocalPosition();
        glm::vec3 targetObjPos = targetObj->transform.GetGlobalPosition();
        //  std::cout << "CamPos: " << camPos.x << "x " << camPos.y << "y " << camPos.z << "z" << std::endl;
        //  std::cout << "targetObjPos: " << targetObjPos.x << "x " << targetObjPos.y << "y " << targetObjPos.z << "z" << std::endl;
        //  std::cout << "DistZ: " << glm::abs(camPos.z - targetObjPos.z) << "z" << std::endl;

        float distanceToObj = glm::distance(camPos, targetObjPos);
        //float distanceRayToObj = glm::distance(rayNorm * distanceToObj + camPos, targetObjPos);
        // std::cout << "DistToObj: " << distanceToObj << std::endl;
       //  std::cout << "Dist Ray: " << distanceRayToObj << std::endl;

        glm::vec3 rayToTargetObjVec = rayNorm * distanceToObj + camPos;
        //std::cout << "rayHit: " << rayToTargetObjVec.x << "x " << rayToTargetObjVec.y << "y " << rayToTargetObjVec.z << "z" << std::endl;

        //rayToTargetObjVec -= targetObjPos;
        //if (glm::abs(rayToTargetObjVec.x) < 2 && glm::abs(rayToTargetObjVec.y) < 2 && glm::abs(rayToTargetObjVec.z) < 2)

        Ray ray(camPos, rayNorm);
        if (intersect(ray, meshNewFromComponent->GetBounds(), targetObj->transform.GetGlobalPosition()))
        {
            if (distanceToObj < nearestDist)
            {
                nearestDist = distanceToObj;
                nearestObj = targetObj.get();
            }
        }

        //if (meshNewFromComponent->GetBounds().Intersects(rayToTargetObjVec, targetObjPos))
        //{
        //    if (distanceRayToObj < nearestDist)
        //    {
        //        nearestDist = distanceRayToObj;
        //        nearestObj = targetObj.get();
        //    }
        //}
    }

    return nearestObj;
}

const std::vector<std::unique_ptr<GameObject>>* const Scene::GetObjectsPtr() const
{
    return &objects;
}

size_t Scene::GetNumberOfWidgets() const
{
    return widgets.size();
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

    for (std::unique_ptr<GameObject>& obj : objects)
    {
        for (std::unique_ptr<Component>& comp : obj->components)
        {
            comp->Start();
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
    widgets.clear();
}

void Scene::Update(const float& deltaTime, glm::mat4 _projection, glm::mat4 _view)
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

        UpdateWidgetInteraction();
    }

    DestroyPostponed();
    Profiler::EndSample();

    //Draw
    sampleName = "Draw time";
    Profiler::BeginSample(sampleName);
    Renderer::DrawNew();
    Profiler::EndSample();

    //Draw UI
    Profiler::BeginSample("UI Draw time");
    Renderer::PrepareDrawUI();
    for (const auto& w : widgets)
    {
        Renderer::DrawUI(w.get());
    }
    Renderer::EndDrawUI();
    Profiler::EndSample();

    //Draw gizmos
    Profiler::BeginSample("Gizmos");
    DrawGizmos();
    Profiler::EndSample();
}

void Scene::UpdateWidgetInteraction()
{
    for (std::unique_ptr<Widget>& widget : widgets)
    {
        glm::vec2 cursorPos = Window::GetCurrentWindow()->GetCursorPositionOnViewport() / Editor::GetViewportSize();
        glm::vec2 widgetPos = widget->GetPosition();
        glm::vec2 widgetSize = widget->GetSize();

        if (cursorPos.x >= widgetPos.x && cursorPos.x <= widgetPos.x + widgetSize.x
            && cursorPos.y >= widgetPos.y && cursorPos.y <= widgetPos.y + widgetSize.y)
        {
            if (!widget->isCursorOn)
            {
                widget->OnCursorEnter();
                widget->isCursorOn = true;
            }

            if(InputManager::GetMouseKeyDown(GLFW_MOUSE_BUTTON_1))
            {
                widget->OnPressed();
            }
            else if (widget->isPressed && InputManager::GetMouseKeyUp(GLFW_MOUSE_BUTTON_1))
            {
                widget->OnReleased();
            }
        }
        else
        {
            if (widget->isCursorOn)
            {
                widget->OnCursorExit();
                widget->isCursorOn = false;
            }

            if (widget->isPressed)
            {
                widget->OnPressedCancel();
            }
        }
    }
}

void Scene::DrawGizmos()
{
    if (Editor::CanDrawGizmos())
    {
        Gizmos::PrepareToRender();
        for (std::unique_ptr<GameObject>& obj : objects)
        {
            if (!obj->IsActive())
                continue;

            obj->ShowOnGizmos();
        }

        Editor::DrawSelectedGameObject();
        NavMesh::DebugDraw();
    }

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
        StaticMeshComponent* spriteComp = obj->AddComponent<StaticMeshComponent>();
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
    //std::cout << "[Copy GameObject] Object size: " << objects.size() << "\n";
    GameObject* orgParent = obj->transform.GetParent();
    if(orgParent != nullptr)
        newObjPtr->transform.SetParent(orgParent);
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
