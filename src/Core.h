#pragma once
#include <memory>
#include "Renderer/Renderer.h"

// Responsible for core loop, resource handling, input handling, etc.
class Core
{
public:
    Core();
    ~Core();

    void Update();

private:
    std::unique_ptr<Renderer> _renderer;
    // Asset Manager
    // Configuration Path
    // Input Manager
};