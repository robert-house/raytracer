#include "Renderer.h"

Renderer::Renderer(unsigned int width, unsigned int height, std::wstring name, std::shared_ptr<Log> log) : Component(log)
{
    std::string description = "";

    description = "Internal width to render at e.g. 1024";
    r_width = Config::getInstance().registerCVar("r_width", 256.0, true, description);

    description = "Internal height to render at e.g. 1024";
    r_height = Config::getInstance().registerCVar("r_height", 256.0, true, description);

  _rendererType = HARDWARE_D3D12;
    // Calculate aspect ratio and explicitly cast to a double
    // TODO: Fix this to pull from the configuration
    _aspectRatio = static_cast<double>(width) / static_cast<double>(height);
    _fullscreen = false;
}

Renderer::~Renderer() {}
