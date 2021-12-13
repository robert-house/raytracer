#include "RenderTarget.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../includes/stb_image_write.h"

// TODO: Rewrite this class because there is alot of bad design decisions here!
// TODO: Add ability to accumulate additional samples over time

// Default Constructor
RenderTarget::RenderTarget()
{
    Init(0, 0);
}

RenderTarget::RenderTarget(unsigned int width, unsigned int height)
{
    Init(width, height);

    // TODO
    // Add assert that both parameters and set variable aspect ratios match?
}

RenderTarget::~RenderTarget() {}

// Preconditions
//          - if any of the input is zero, post warning message and use hardcoded defaults
//          - both inputs must be >=0
void RenderTarget::Init(unsigned int width, unsigned int height)
{
    if (width == 0 || height == 0)
    {
        _width = DEFAULT_WIDTH;
        _height = DEFAULT_HEIGHT;
    }
    else
    {
        _width  = width;
        _height = height;
    }

    // Calculate aspect ratio here
    _aspectRatio = double(_width / _height);

    // Clear render target data and set bounds for it
    _targetData.clear();

    // I set the width as the inner dimension to keep the horizontal pixels contiguous in memory.
    // This will allow the beam to sweep from "left" to "right" and not thrash the hell out of the
    // cache.
    _targetData = std::vector<std::vector<Vec3>>(_height, std::vector<Vec3>(_width));
}

// TODO: flat datastructure using algorithm x + stride * y
//       where stride is the resolution width
void RenderTarget::draw(int y, int x, Vec3 color)
{
    _targetData[y][x] = Vec3(color.x(), color.y(), color.z());
}

Vec3 RenderTarget::get(int y, int x) const
{
    return _targetData[y][x];
}

void RenderTarget::accumulate(int y, int x, Vec3 color)
{
    _targetData[y][x] = _targetData[y][x] + color;
}

void RenderTarget::writeFrameToDisk(std::string path)
{
    writePNG(path);
    clear();
}

unsigned int RenderTarget::getHeight()
{
    return _height;
}

unsigned int RenderTarget::getWidth()
{
    return _width;
}

RenderTarget& RenderTarget::operator+=(const RenderTarget& right)
{
    // Take elements from right and add to this
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            _targetData[j][i] += right.get(j,i);
        }
    }
}

// Private Members
bool RenderTarget::resizeTarget(unsigned int width, unsigned int height)
{
    // TODO: Hook up Log object here!
    std::cout << "Resizing RenderTarget to: " << width << " x " << height << std::endl;

    // Call Init again because it makes sense at this point
    Init(width, height);

    return true;
}

// Private functions
void RenderTarget::writePPM(std::string path)
{
    std::ofstream file;

    if (path == "")
    {
        // Give default and warn user
        path = "frame.ppm";
        // TODO: WARNING LOGS
    }

    file.open(path);

    // Write out the header
    file << "P3\n" << _width << " " << _height << "\n255\n";

    // Traverse the rendertarget and print out the frame
    for (int j = _height - 1; j >= 0; j--)
    {
        for (int i = 0; i < _width; i++)
        {
            // Do actual write to file
            file << _targetData[j][i].x()
                << " "
                << _targetData[j][i].y()
                << " "
                << _targetData[j][i].z()
                << "\n";
        }
    }

    file.close();
}

void RenderTarget::writePNG(std::string path)
{
        std::string pathWithExt = path + ".png";

    // For debugging purposes, have to unroll all of this
    std::vector<unsigned char> image;

    for (int j = _height - 1; j >= 0; j--)
    {
        for (int i = 0; i < _width; i++)
        {
            image.push_back(_targetData[j][i].x());
            image.push_back(_targetData[j][i].y());
            image.push_back(_targetData[j][i].z());
        }
    }
    //stbi_write_png(fileName.c_str(), _width, _height, 3, image.data(), _width * sizeof(uint32_t));
    stbi_write_png(pathWithExt.c_str(), _width, _height, 3, image.data(), _width * 3);

    // TODO: Make this a log output
    std::cout << "File Writen to Disk..." << pathWithExt << std::endl;
}

void RenderTarget::clear()
{
    // Traverse the rendertarget and print out the frame
    for (int j = _height - 1; j >= 0; j--)
    {
        for (int i = 0; i < _width; i++)
        {
            _targetData[j][i] = Vec3(0.0, 0.0, 0.0);
        }
    }
}
