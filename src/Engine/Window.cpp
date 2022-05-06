#include <Window.hpp>


void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    std::cout << "Length: " << length << " User Param: " << userParam << std::endl;
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

Window::Window(IncuhState *state) : mainState(state){
    __width = mainState->mainArgs->getArguments()->width;
    __height = mainState->mainArgs->getArguments()->height;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0){
        printf("Failed to initalize SDL: %s\n", SDL_GetError());
    }
    __win = SDL_CreateWindow(state->mainArgs->getArguments()->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, __width, __height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (__win == NULL){
        printf("Failed to create SDL Window: %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);


    __con = SDL_GL_CreateContext(__win);
    SDL_GL_MakeCurrent(__win, __con);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    glViewport(0,0,__width,__height);
    SDL_GL_SetSwapInterval(0);
    //SDL_SetWindowFullscreen(__win, 1);

    SDL_ShowCursor(SDL_ENABLE);
    //SDL_SetWindowGrab(__win, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_DEBUG_OUTPUT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glFrontFace(GL_CW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    __mainUI = new UI_Render(__win, __con);

#ifndef __WIN32__
//For some reason, this doesn't work in my Windows VM, so keeping it disabled for now
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        printf("Successfully enabled debug output\n");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);

        #ifndef NDEBUG
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_TRUE);
        #else
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
        #endif // NDEBUG
    }
#endif // __WIN32__
    e.type = 0;
    __mainUI->newFrame();
}
Window::~Window(){
  SDL_GL_DeleteContext(__con);
  SDL_DestroyWindow(__win);
  SDL_Quit();
}

void Window::update(){
  __mainUI->render();
  SDL_GL_SwapWindow(__win);
  SDL_UpdateWindowSurface(__win);
  //SDL_PollEvent(&e);
  //ImGui_ImplSDL2_ProcessEvent(&e);
  //ImGui_ImplSDL2_ProcessEvent(&e);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  __mainUI->newFrame();
}

int Window::getCloseState(){
  return (e.type == SDL_QUIT);
}

std::vector<uint8_t> Window::getActiveKeys(){
  std::vector<uint8_t> keys;
  Bindings::resetState();
  while (SDL_PollEvent(&e))
  {

      __mainUI->updateEvents(&e);
      if (e.type == SDL_KEYUP) {
          //mainState->mainBindings->callBackUp(e.key.keysym.scancode);
          Bindings::callBackUp(e.key.keysym.scancode);
      }
      switch (e.type){
          case SDL_KEYUP:
              Bindings::registerKeyUp(e.key.keysym.scancode);
              break;
          case SDL_KEYDOWN:
              Bindings::registerKeyDown(e.key.keysym.scancode);
              break;
          default:
              break;
      }

  }
  __keys = SDL_GetKeyboardState(&__key_len);
  for (int x = 0; x<__key_len; x++){
    if (__keys[x]){
        Bindings::callBackDown(x);
        Bindings::registerKey(x);
    }
  }
  return keys;
}

SDL_Window* Window::getSDLWindow(){
    return __win;
}

void Window::resetCursor(uint32_t newX, uint32_t newY){
    SDL_WarpMouseInWindow(__win, newX, newY);
}

uint32_t Window::getWidth(){ return __width; }
uint32_t Window::getHeight() { return __height; }
