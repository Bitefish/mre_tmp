#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "../imgui.h"
#include "../backends/imgui_impl_sdl2.h"
#include "../backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif


using namespace std;
bool init_state = false;
bool fullscreen = false;
SDL_Window* window;
SDL_GLContext gl_context;
SDL_WindowFlags window_flags;
ImVec4 clear_color;
bool fullScreen = false;
std::string txtBox = "no click";

bool loopEventListener() {

    SDL_Event event;
    bool done = false;
    while (SDL_PollEvent(&event)) {
        //ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym){
                case 'f':
                    fullScreen = !fullScreen;
                    if (fullScreen)
                    {
                        SDL_SetWindowFullscreen(window, window_flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                    }
                    else
                    {
                        SDL_SetWindowFullscreen(window, window_flags);
                    }
                    break;
                default:
                    break;
            }
        }
        else if (event.type == SDL_WINDOWEVENT &&
                 event.window.event == SDL_WINDOWEVENT_CLOSE &&
                 event.window.windowID == SDL_GetWindowID(window)){
            done = true;
        }

        /*
       else if (event.type == SDL_QUIT){
           done = true;
       }
        */
    }

    return done;

}


int main(int, char**) {



    std::string GL_sl_version;
    bool window_active = true;

    ImGuiWindowFlags imgui_window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;

    ImVec2 window_size = ImVec2(640.0F, 480.0F);
    ImVec2 window_pos = ImVec2(0.0F, 0.0F);

    //ImGuiIO* io;



    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // dynamic contents
    //gain=1.0f;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    GL_sl_version = std::move("#version 100");

#ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


    std::string ctLabel = "first";


    window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    const char* window_title = "MRE";
    float width = 840.0F;
    float height = 520.0F;

    window_size = ImVec2(width, height);
    window_pos = ImVec2(0.0F, 0.0F);
    window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);

    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        exit(1);
    }

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);  // Enable vsync

    //clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(GL_sl_version.c_str());


    while (!loopEventListener()) {



        ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplSDL2_NewFrame(window);
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        //std::cout << imgui_window_flags << std::endl;

        int width = 0;
        int height = 0;

        SDL_GetWindowSize(window, &width, &height);
        //std::cout << static_cast<float>(width) << "x" << static_cast<float>(height) << std::endl;
        window_size = ImVec2(static_cast<float>(width), static_cast<float>(height));

        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowPos(window_pos);

        ImGui::Begin("IML", &window_active, imgui_window_flags); //


        bool check = false;

        if (ImGui::Button(ctLabel.c_str(), ImVec2(90,30)) || ImGui::IsItemClicked()) {
            std::cout << "draw.action: " << std::endl;
            check = true;
            txtBox = "clicked it!";
        }

        if (check) {
            std::cout << "draw.action: TRUE" << std::endl;
        }

        ImGui::Text(txtBox.c_str());


        ImGui::End();
        ImGui::Render();
        SDL_GL_MakeCurrent(window, gl_context);

        //glViewport(0, 0, (int) io->DisplaySize.x, (int) io->DisplaySize.y); // VIDEO? //
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);



    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

  return 0;
}