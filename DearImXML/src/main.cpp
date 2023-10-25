
#include <GL/gl3w.h>            // Initialize with gl3wInit()
// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_internal.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>

#include <string>
#include <iostream>
#include <fstream>

#include <XMLReader.h>
#include <XMLTree.h>
#include <XMLRenderer.h>
#include <XMLEventHandler.h>
#include <XMLDynamicBind.h>

class Handler : public ImXML::XMLEventHandler {
    virtual void onNodeBegin(ImXML::XMLNode& node) override {

    }
    
    virtual void onNodeEnd(ImXML::XMLNode& node) override {

    }

    virtual void onEvent(ImXML::XMLNode& node) override {
        if(node.args["id"] == "btn0") {
            node.args["label"] = "Clicked";
        }
    }

};

int main(int argc, char const *argv[])
{
	if (!glfwInit())
    	return 1;

    // Decide GL+GLSL versions
#ifdef __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 330 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 330 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ICE Engine", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    bool err = gl3wInit();

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

	ImXML::XMLReader reader = ImXML::XMLReader();
	ImXML::XMLTree tree = reader.read("Assets/test.xml");
    ImXML::XMLRenderer renderer;
    float float0;
    char buf[512] = {0};
    float color0[3] = {0};
    float color1[4] = {0};
    renderer.addDynamicBind(std::string("float0"), {.ptr = &float0});
    renderer.addDynamicBind(std::string("str0"), {.ptr = buf, .size=512});
    renderer.addDynamicBind(std::string("color0"), {.ptr = color0});
    renderer.addDynamicBind(std::string("color1"), {.ptr = color1});
    Handler handler;

	while (!glfwWindowShouldClose(window))
    {
        IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render your GUI
        renderer.render(tree, handler);

        // Render dear imgui into screen
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    
	return 0;
}
