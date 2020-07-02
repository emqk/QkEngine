#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Editor.h"
#include "Scene.h"
#include "Window.h"
#include "Gizmos.h"
#include "InputManager.h"

#include <vector>
#include <memory>

GameObject* Editor::selectedObj;
Component* Editor::selectedComp;
std::function<void(const std::string&)> Editor::currSelectAssetFun;
AssetWindowType Editor::currAssetWindowType;
bool Editor::showSelectAssetWindow;
bool Editor::isAnyWindowOrItemHovered;
bool Editor::drawGizmos = true;

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
        std::string assetTypeStr;
        std::vector<std::string> assetsName;
        if (currAssetWindowType == AssetWindowType::Textures)
        {
            assetsName = ResourceManager::GetTexturesName();
            assetTypeStr = "Textures";
        }
        else if (currAssetWindowType == AssetWindowType::MeshesNew)
        {
            assetsName = ResourceManager::GetMeshesNewName();
            assetTypeStr = "MeshesNew";
        }
        else if (currAssetWindowType == AssetWindowType::Shaders)
        {
            assetsName = ResourceManager::GetShadersName();
            assetTypeStr = "Shaders";
        }
       
        if (ImGui::TreeNode(assetTypeStr.c_str()))
        {
            for (size_t n = 0; n < assetsName.size(); n++)
            {
                if (currAssetWindowType == AssetWindowType::Textures)
                {
                    Texture* tex = ResourceManager::GetTexture(assetsName[n].c_str());
                    ImGui::Image((void*)(intptr_t)tex->GetID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
                }

                char buf[128];
                sprintf_s(buf, "%d. %s", n, assetsName[n].c_str());
                if (ImGui::Selectable(buf, selectedAsset == n, ImGuiSelectableFlags_AllowDoubleClick))
                {
                    selectedAsset = n;
                    if (ImGui::IsMouseDoubleClicked(0))
                    {
                        currSelectAssetFun(assetsName[selectedAsset].c_str());
                        showSelectAssetWindow = false;
                    }
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::Button("Select") && selectedAsset >= 0 && selectedAsset < assetsName.size())
        {
            currSelectAssetFun(assetsName[selectedAsset].c_str());
            showSelectAssetWindow = false;
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
                if (ImGui::MenuItem("Empty", "Ctrl+E"))
                {
                    GameObject* newObj = Scene::GetCurrentScene().Instantiate<GameObject>(glm::vec3(0, 0, 0));
                    newObj->name = "empty";
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
        ImGui::SameLine();
        ImGui::Checkbox("Draw Gizmos", &drawGizmos);

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
        ImGui::Text("Last frame vertices: %u", Renderer::GetDrawnVerticesLastFrame());
        ImGui::End();
    }

    if (selectedObj != nullptr)
    {
        SpriteComponent* spriteComp = selectedObj->GetComponent<SpriteComponent>();
        if (spriteComp)
        {
            Mesh* mesh = spriteComp->GetMeshNew();
            if (mesh != nullptr)
            {
                glm::vec3 position = selectedObj->GetPosition();
                Bounds bounds = mesh->GetBounds();
                Gizmos::SetCurrentColor(Gizmos::meshWireframeColor);
                Gizmos::DrawMeshNewWireframe(position, selectedObj->GetRotation(), selectedObj->GetScale() * 1.001f, *mesh);
            }
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    isAnyWindowOrItemHovered = ImGui::IsAnyWindowOrItemHovered() || ImGui::IsAnyItemHovered();

    if (Scene::GetCurrentScene().IsInGameMode())
    {
        //Show Cursor 
        if (InputManager::GetKeyDown(GLFW_KEY_F1))
        {
            Window::GetCurrentWindow()->SetCursorMode(GLFW_CURSOR_NORMAL);
        }
        //Hide cursor
        else if (InputManager::GetMouseKeyDown(GLFW_MOUSE_BUTTON_1) && !isAnyWindowOrItemHovered)
        {
            Window::GetCurrentWindow()->SetCursorMode(GLFW_CURSOR_DISABLED);
        }
        if (InputManager::GetKeyDown(GLFW_KEY_ESCAPE))
        {
            ExitGameMode();
        }
    }
    
}

void Editor::Select(GameObject* obj)
{
    selectedObj = obj;
}

bool Editor::CanDrawGizmos()
{
    return drawGizmos;
}

bool Editor::IsAnyWindowOrItemHovered()
{
    return isAnyWindowOrItemHovered;
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
    glfwSetInputMode(&Window::GetCurrentWindow()->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Editor::ExitGameMode()
{
    Scene::GetCurrentScene().ExitGameMode();
    glfwSetInputMode(&Window::GetCurrentWindow()->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}