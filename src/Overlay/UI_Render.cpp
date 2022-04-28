#include <Overlay/UI_Render.hpp>

UI_Render::UI_Render(SDL_Window *win, SDL_GLContext con) : uWin(win), uContext(con)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;


    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGui_ImplSDL2_InitForOpenGL(uWin, uContext);
    ImGui_ImplOpenGL3_Init("#version 440 core");

}

UI_Render::~UI_Render()
{
    //dtor
}

void UI_Render::updateEvents(SDL_Event *e){
    ImGui_ImplSDL2_ProcessEvent(e);
}


void UI_Render::newFrame(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

}
void UI_Render::render(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
