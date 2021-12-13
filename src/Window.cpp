#include "Window.h"

Window::Window()
{
    // Always check to see whether the CVar has been registered or not before
    // registering them. registerCVar() already handles this, but will throw
    // a warning as it is NOT best practice.

    // TODO: investigate this as it can get really messy pretty quickly
    //       probably should not try to register this particular cvar here if
    //       I already know its in renderer

    // Should rendering resolution be tied to the window resolution? No
    w_width  = Config::getInstance().getCVar("w_width");
    w_height = Config::getInstance().getCVar("w_height");

    if (w_width == nullptr)
    {
        w_width = Config::getInstance().registerCVar("w_width", 512, true, "Window width. different than rendering width");
    }

    if (w_height == nullptr)
    {
        w_height = Config::getInstance().registerCVar("w_height", 512, true, "Window height. Different than rendering height");
    }

    // Init other pointers
    _pWindow = nullptr;
    _pRenderer = nullptr;
    _pWindowTexture = nullptr;

    init();
}

Window::~Window()
{
    SDL_DestroyTexture(_pWindowTexture);
    SDL_DestroyRenderer(_pRenderer);
    SDL_DestroyWindow(_pWindow);
    SDL_Quit();
    std::cout << "[Window] -- Shutdown was successfully completed" << std::endl; // TODO: convert to log
}

bool Window::init()
{
    // Also include reinit

    //pLog->log(Log::INFO, "SDL2 Has Initialized");
    setupSDLWindow();
    setupSDLRenderer();
    sdlLoadTestImage();
}

void Window::draw()
{
    SDL_RenderClear(_pRenderer);
    SDL_RenderCopy(_pRenderer, _pWindowTexture, NULL, NULL);
    SDL_RenderPresent(_pRenderer);
}

// TODO: Implement
void Window::loadFrameBuffer(RenderTarget* pRenderTarget)
{
    // Release current texture

    // TODO: RenderTarget -> SDL_Texture
    _pWindowTexture = SDL_CreateTexture(_pRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 1024, 768);

    // Do rendertarget copy here?

}

bool Window::setupSDLWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_INIT ERROR: " << SDL_GetError() << std::endl;
        return false;
    }

    _pWindow = SDL_CreateWindow("RUNIC",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                w_width->getValue(),
                                w_height->getValue(),
                                SDL_WINDOW_SHOWN);

    if (_pWindow == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // TODO: log message saying window init properly
    return true;
}

bool Window::setupSDLRenderer()
{
    // Do renderer init
    _pRenderer = SDL_CreateRenderer(_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (_pRenderer == nullptr)
    {
        SDL_DestroyWindow(_pWindow);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    return true;
}

bool Window::sdlLoadTestImage()
{
    std::string imagePath = "./hello.bmp";
    SDL_Surface* bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr)
    {
        SDL_DestroyRenderer(_pRenderer);
        SDL_DestroyWindow(_pWindow);
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    _pWindowTexture = SDL_CreateTextureFromSurface(_pRenderer, bmp);
    SDL_FreeSurface(bmp);
    if (_pWindowTexture == nullptr)
    {
        SDL_DestroyRenderer(_pRenderer);
        SDL_DestroyWindow(_pWindow);
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    return true;
}

// TODO: move this to main()
void Window::sdlLoop()
{
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN)
            {
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                quit = true;
            }
        }
    }
}