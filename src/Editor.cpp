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
    ImGui::ShowDemoWindow();

    Scene& currentScene = Scene::GetCurrentScene();

    //Test
    ImGui::Begin("HierarchyWithParents");
    if (ImGui::TreeNode("Advanced, with Selectable nodes"))
    {
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        static bool align_label_with_current_x_position = false;
        if (align_label_with_current_x_position)
            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

        static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
        int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
        
        const std::vector<std::unique_ptr<GameObject>>* const objects = currentScene.GetObjectsPtr();
        for (size_t n = 0; n < objects->size(); n++)
        {
            // Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
            ImGuiTreeNodeFlags node_flags = base_flags;
            const bool is_selected = (selection_mask & (1 << n)) != 0;
            if (is_selected)
                node_flags |= ImGuiTreeNodeFlags_Selected;

            //
            char buf[64];
            sprintf_s(buf, "%d. %s", n, (*objects)[n]->name.c_str());
           
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)n, node_flags, buf);
            if (ImGui::IsItemClicked())
            {
                node_clicked = n;
                selectedObj = (*objects)[node_clicked].get();
            }
            if (node_open)
            {
                if ((*objects)[n].get()->childs.size() > 0)
                {
                    node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                    ImGui::TreeNodeEx((void*)(intptr_t)n, node_flags, (*objects)[n].get()->childs[0]->name.c_str());
                    if (ImGui::IsItemClicked())
                        selectedObj = (*objects)[n].get()->childs[0];
                }
            }

          /*  if (ImGui::Selectable(buf, node_clicked == n))
            {
                node_clicked = n;
                selectedObj = (*objects)[n].get();
            }*/


            //else
            //{
            //    // Items 3..5 are Tree Leaves
            //    // The only reason we use TreeNode at all is to allow selection of the leaf.
            //    // Otherwise we can use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
            //    node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
            //    ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
            //    if (ImGui::IsItemClicked())
            //        node_clicked = i;
            //}
        }
        
        //for (int i = 0; i < 6; i++)
        //{
        //    // Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
        //    ImGuiTreeNodeFlags node_flags = base_flags;
        //    const bool is_selected = (selection_mask & (1 << i)) != 0;
        //    if (is_selected)
        //        node_flags |= ImGuiTreeNodeFlags_Selected;
        //    if (i < 3)
        //    {
        //        // Items 0..2 are Tree Node
        //        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
        //        if (ImGui::IsItemClicked())
        //            node_clicked = i;
        //        if (node_open)
        //        {
        //            ImGui::BulletText("Blah blah\nBlah Blah");
        //            ImGui::TreePop();
        //        }
        //    }
        //    else
        //    {
        //        // Items 3..5 are Tree Leaves
        //        // The only reason we use TreeNode at all is to allow selection of the leaf.
        //        // Otherwise we can use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
        //        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
        //        ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
        //        if (ImGui::IsItemClicked())
        //            node_clicked = i;
        //    }
        //}
        if (node_clicked != -1)
        {
            // Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
            if (ImGui::GetIO().KeyCtrl)
                selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
            else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
                selection_mask = (1 << node_clicked);           // Click to single-select
        }
        if (align_label_with_current_x_position)
            ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::TreePop();
    }
    ImGui::End();

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
                glm::vec3 localPosition = selectedObj->GetLocalPosition();
                Bounds bounds = mesh->GetBounds();
                Gizmos::SetCurrentColor(Gizmos::meshWireframeColor);
                Gizmos::DrawMeshNewWireframe(localPosition, selectedObj->GetLocalRotation(), selectedObj->GetLocalScale() * 1.001f, *mesh);
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