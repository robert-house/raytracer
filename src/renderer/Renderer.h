#pragma once

#include <string>
#include <memory>
#include "../Component.h"
#include "../logging/Log.h"

class Renderer : public Component
{
public:
	Renderer(); // Default Constructor
    Renderer(unsigned int width, unsigned int height, std::wstring name, std::shared_ptr<Log> log);
	virtual ~Renderer();

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void destroy() = 0;
    virtual void reInit() = 0;

	// Accessors
    unsigned int getWidth() const { return r_width->getValue(); }
    unsigned int getHeight() const { return r_height->getValue(); }

    // Mutators
    virtual void changeResolution(int width, int height) = 0;

protected:
    CVar* r_width;
    CVar* r_height;
    double       _aspectRatio;
    bool         _fullscreen;

    // Renderer Type enum so we can choose what type of rendering we
    // want to do
    enum RendererType
    {
        HARDWARE_D3D12,
        HARDWARE_VK,
        SOFTWARE_RT,
        HARDWARE_RT_VK,
        HARDWARE_RT_D3DRT,
        SOFTWARE
    };

    // Adaptor Info
    RendererType _rendererType;

private:
    // Root Asset Path
    std::wstring _assetPath; // TODO: Figure out if this is with respect to the *.exe location
};