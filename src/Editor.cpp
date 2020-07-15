#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Editor.h"
#include "Scene.h"
#include "Window.h"
#include "Gizmos.h"
#include "InputManager.h"
#include "Profiler.h"
#include "Lighting.h"

#include <memory>
#include "imgui/imgui_internal.h"

GameObject* Editor::selectedObj;
Component* Editor::selectedComp;
std::function<void(const std::string&)> Editor::currSelectAssetFun;
AssetWindowType Editor::currAssetWindowType;
bool Editor::isAnyWindowOrItemHovered;
bool Editor::drawGizmos = true;
GameObject* Editor::currentlyDraggedHierarchyObj = nullptr;

bool Editor::showHierarchy = true;
bool Editor::showInspector = true;
bool Editor::showLightingWindow = true;
bool Editor::showProfiler = false;
bool Editor::showSelectAssetWindow = false;
bool Editor::showCameraWindow = true;


void Editor::Init(GLFWwindow* window)
{
    //Docking
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    //Load default font
    ImFont* pFont = io.Fonts->AddFontFromFileTTF("../QkEngine/Resources/Fonts/Roboto-Regular.ttf", 14.0f);

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }


    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(0x82E9));
}

void Editor::Update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();


    ShowEnabledWindows();


    if (selectedObj != nullptr)
    {
        SpriteComponent* spriteComp = selectedObj->GetComponent<SpriteComponent>();
        if (spriteComp)
        {
            Mesh* mesh = spriteComp->GetMeshNew();
            if (mesh != nullptr)
            {
                glm::vec3 localPosition = selectedObj->transform.GetLocalPosition();
                Bounds bounds = mesh->GetBounds();
                Gizmos::SetCurrentColor(Gizmos::meshWireframeColor);
                Gizmos::DrawMeshNewWireframe(
                      selectedObj->transform.GetGlobalPosition()
                    , selectedObj->transform.GetGlobalEulerAngles()
                    , selectedObj->transform.GetGlobalScale() * 1.001f
                    , *mesh);
            }
        }
    }


    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    isAnyWindowOrItemHovered = ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemHovered();

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

void Editor::ShowEnabledWindows()
{
    //Camera
    if(showCameraWindow)
    {
        ImGui::Begin("Camera", &showCameraWindow);
        Scene::GetCurrentScene().GetCamera().ShowOnInspector();
        ImGui::End();
    }

    if(showHierarchy)
        ShowHierarchy();
    
    if(showInspector)
        ShowInspector();

    if (showSelectAssetWindow)
        ShowSelectAssetWindow();

    if (showLightingWindow)
        Lighting::ShowWindow();

    if (showProfiler)
        Profiler::ShowData();
    

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
            if (ImGui::BeginMenu("Windows"))
            {
                if (ImGui::MenuItem("Inspector"))
                {
                    showInspector = !showInspector;
                }
                if (ImGui::MenuItem("Hierarchy"))
                {
                    showHierarchy = !showHierarchy;
                }
                if (ImGui::MenuItem("Lighting"))
                {
                    showLightingWindow = !showLightingWindow;
                }
                if (ImGui::MenuItem("Profiler"))
                {
                    showProfiler = !showProfiler;
                }
                if (ImGui::MenuItem("Camera settings"))
                {
                    showCameraWindow = !showCameraWindow;
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

        ImGui::End();
    }
}

void Editor::ShowInspector()
{
    ImGui::Begin("Inspector", &showInspector);
    
    if (selectedObj != nullptr)
    {
        selectedObj->ShowOnInspector(selectedObj, selectedComp);
    }
    
    ImGui::End();   
}


void Editor::ShowHierarchy()
{
    Scene& currentScene = Scene::GetCurrentScene();
    const std::vector<std::unique_ptr<GameObject>> const* objects = currentScene.GetObjectsPtr();

    //Test
    ImGui::Begin("Hierarchy", &showHierarchy);

    if (ImGui::TreeNode("Scene"))
    {
        static int node_clicked = -1;
        int id = -1;
        for (int i = 0; i < objects->size(); i++)
        {           
            if ((*objects)[i].get()->transform.GetParent() == nullptr)
            {
                ShowGameObject((*objects)[i].get(), id, node_clicked);
            }
        }

        ImGui::TreePop();
    }

    ImGui::End();

    //Null currently dragged object if grabbing nothing
    if (currentlyDraggedHierarchyObj != nullptr)
    {
        if (!ImGui::IsMouseDragging(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            NullCurrentlyDraggedHierarchyObj();
        }
    }
}


void Editor::ShowGameObject(GameObject* obj, int& id, int& node_clicked)
{
    id++;
    const char* objName = obj->name.c_str();
    static constexpr ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    ImGuiTreeNodeFlags flags = base_flags;

    if (id == node_clicked)
        flags |= ImGuiTreeNodeFlags_Selected;

    if (obj->transform.GetChilds().size() <= 0)
    {
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet

        ImGui::TreeNodeEx((void*)(intptr_t)id, flags, objName);
        if (ImGui::IsItemClicked())
        {
            node_clicked = id;
            Select(obj);
        }

        Drag(obj);
    }
    else
    {
        bool isOpen = ImGui::TreeNodeEx((void*)(intptr_t)id, flags, objName);
        if (ImGui::IsItemClicked())
        {
            node_clicked = id;
            Select(obj);
        }

        Drag(obj);

        if (isOpen)
        {
            for (size_t i = 0; i < obj->transform.GetChilds().size(); i++)
            {
                ShowGameObject(obj->transform.GetChilds()[i], id, node_clicked);
            }
            ImGui::TreePop();
        }
    }
}

void Editor::ShowSelectAssetWindow()
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

void Editor::Drag(GameObject* obj)
{    
    static int id = 0;

    ImGui::PushID(id);
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        // Set payload to carry the index of our item (could be anything)
        ImGui::SetDragDropPayload("HObjElement", &id, sizeof(int));
        if (obj != nullptr)
        {
            if (currentlyDraggedHierarchyObj == nullptr)
            {
                currentlyDraggedHierarchyObj = obj;
                std::cout << "Begin drag obj set to: " << currentlyDraggedHierarchyObj->name << std::endl;
            }
            if (currentlyDraggedHierarchyObj != nullptr)
            {
                ImGui::Text("Set parent of: %s", currentlyDraggedHierarchyObj->name.c_str());
            }
        }
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (currentlyDraggedHierarchyObj != nullptr && obj != nullptr)
        {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HObjElement");
            if (payload)
            {
                if (!obj->transform.IsObjectOneOfMyParents(currentlyDraggedHierarchyObj))
                {
                    currentlyDraggedHierarchyObj->transform.SetParent(obj);
                    NullCurrentlyDraggedHierarchyObj();
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::PopID();
}

void Editor::NullCurrentlyDraggedHierarchyObj()
{
    std::cout << "Dragging ended!\n";
    currentlyDraggedHierarchyObj = nullptr;
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
    Select(nullptr);
    Scene::GetCurrentScene().EnterGameMode();
    glfwSetInputMode(&Window::GetCurrentWindow()->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Editor::ExitGameMode()
{
    Scene::GetCurrentScene().ExitGameMode();
    glfwSetInputMode(&Window::GetCurrentWindow()->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}