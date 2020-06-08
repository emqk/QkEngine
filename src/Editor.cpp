#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Editor.h"
#include "Scene.h"
#include "Window.h"

#include <vector>
#include <memory>

GameObject* Editor::selectedObj;
Component* Editor::selectedComp;
std::function<void(const std::string&)> Editor::currSelectAssetFun;
AssetWindowType Editor::currAssetWindowType;
bool Editor::showSelectAssetWindow;

Editor::Editor()
{
   
}

Editor::~Editor()
{
}

void Editor::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(0x82E9));
}


void Editor::Update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();

    Scene& currentScene = Scene::GetCurrentScene();

    //Camera
    {
        ImGui::Begin("Camera");
        currentScene.GetCamera().ShowOnInspector();
        ImGui::End();
    }

    //Hierarchy
    {
        ImGui::Begin("Hierarchy");
        
        if (ImGui::TreeNode("World"))
        {
            static int selectedGOInHierarchy = -1;
            const std::vector<std::unique_ptr<GameObject>>* const objects = currentScene.GetObjectsPtr();
            for (size_t n = 0; n < objects->size(); n++)
            {
                char buf[64];
                sprintf_s(buf, "%d. %s", n, (*objects)[n]->name.c_str());
                if (ImGui::Selectable(buf, selectedGOInHierarchy == n))
                {
                    selectedGOInHierarchy = n;
                    selectedObj = (*objects)[selectedGOInHierarchy].get();
                }
            }
            ImGui::TreePop();
        }

        ImGui::End();
    }

    //Inspector
    {
        ImGui::Begin("Inspector");

        if (selectedObj != nullptr)
        {
            selectedObj->ShowOnInspector(selectedObj, selectedComp);
        }

        ImGui::End();
    }

    //SetAsset
    if(showSelectAssetWindow)
    {
        ImGui::Begin("Select Asset", &showSelectAssetWindow, ImGuiWindowFlags_MenuBar);
    
        static int selectedAsset = -1;
        std::vector<std::string> vec;
        if (currAssetWindowType == AssetWindowType::Textures)
            vec = ResourceManager::GetTexturesName();
        else if(currAssetWindowType == AssetWindowType::Meshes)
            vec = ResourceManager::GetMeshesName();
        else if (currAssetWindowType == AssetWindowType::Shaders)
            vec = ResourceManager::GetShadersName();

        if (ImGui::TreeNode("World"))
        {
            for (size_t n = 0; n < vec.size(); n++)
            {
                char buf[128];
                sprintf_s(buf, "%d. %s", n, vec[n].c_str());
                if (ImGui::Selectable(buf, selectedAsset == n))
                {
                    selectedAsset = n;
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::Button("Select") && selectedAsset >= 0 && selectedAsset < vec.size())
        {
            std::cout << "Selected: " << vec[selectedAsset] << std::endl;
            currSelectAssetFun(vec[selectedAsset].c_str());
        }

        ImGui::End();
    }

    //Tools
    {
        static bool isToolActive = true;
        ImGui::Begin("Tools", &isToolActive, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Create"))
            {
                if (ImGui::MenuItem("GameObject", "Ctrl+G"))
                { 
                    GameObject* newObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(-10, 10, 0));
                    SpriteComponent* spriteComp = newObj->AddComponent<SpriteComponent>();
                    spriteComp->SetShader(ResourceManager::GetShader("StandardShader"));
                    spriteComp->SetTexture(ResourceManager::GetTexture("Grocery.png"));
                    spriteComp->SetMesh("Grocery.obj");
                }
                if (ImGui::MenuItem("Empty", "Ctrl+E"))
                {
                    GameObject* newObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(-10, 10, 0));
                    newObj->name = "empty";
                }
                if (ImGui::MenuItem("Ground"))
                {
                    GameObject* newObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(-10, 10, 0));
                    SpriteComponent* spriteComp = newObj->AddComponent<SpriteComponent>();
                    spriteComp->SetShader(ResourceManager::GetShader("StandardShader"));
                    spriteComp->SetTexture(ResourceManager::GetTexture("kenney_medievalrtspack/PNG/Retina/Tile/asd.png"));
                    spriteComp->SetMesh("Human/Human.obj");
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Application"))
            {
                if (ImGui::MenuItem("Quit"))
                {
                    glfwSetWindowShouldClose(&Window::GetCurrentWindow()->GetGLFWWindow(), true);
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Text("Draw mode");
        if (ImGui::Button("Solid"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        ImGui::SameLine();
        if (ImGui::Button("Wireframe"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        ImGui::SameLine();
        if (ImGui::Button("Points"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }

        if (Scene::IsInGameMode())
            ImGui::Text("IN PlayMode");
        else
            ImGui::Text("IN EditorMode");

        ImGui::SameLine();
        if (ImGui::Button("Play"))
        {
            EnterGameMode();
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {
            ExitGameMode();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)\n", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Number of GameObjects: %u", currentScene.GetObjectsPtr()->size());
        ImGui::Text("Last frame draw calls: %u | To draw container size: %u", Renderer::GetDrawCallsLastFrame(), Renderer::GetToDrawContainerSize());
        ImGui::Text("Last frame triangles: %u", Renderer::GetDrawnTrianglesLastFrame());
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Select(GameObject* obj)
{
    selectedObj = obj;
}

void Editor::ShowSelectAssetWindow(const AssetWindowType& assetWindowType, const std::function<void(std::string)>& fun)
{
    currAssetWindowType = assetWindowType;
    currSelectAssetFun = fun;
    showSelectAssetWindow = true;
}

void Editor::EnterGameMode()
{
    Scene::GetCurrentScene().EnterGameMode();
}

void Editor::ExitGameMode()
{
    Scene::GetCurrentScene().ExitGameMode();
}