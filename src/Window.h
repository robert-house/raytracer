#include "config/Config.h"
#include "renderer/RenderTarget.h"
// SDL2 headers
#include <SDL2/SDL.h>

class Window
{
public:
    Window();
    ~Window();

    bool init();
    void sdlLoop();

    void draw();
    void loadFrameBuffer(RenderTarget* pRenderTarget);

private:
    CVar* w_width;
    CVar* w_height;

    // SDL Specific stuff
    // TODO: abstract
    SDL_Window* _pWindow;
    SDL_Renderer* _pRenderer;
    SDL_Texture* _pWindowTexture;


    bool setupSDLWindow();
    bool setupSDLRenderer();
    bool sdlLoadTestImage();
};