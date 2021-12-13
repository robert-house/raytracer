#pragma once
#include <vector>
#include <fstream>
#include "../Vec3.h"
#include "../Color.h"

class RenderTarget
{
public:
    RenderTarget();
    RenderTarget(unsigned int width, unsigned int height);
    ~RenderTarget();

    bool resizeTarget(unsigned int width, unsigned int height);
    void draw(int x, int y, Vec3 color);
    Vec3 get(int x, int y) const;
    void accumulate(int x, int y, Vec3 color);
    void writeFrameToDisk(std::string path);
    unsigned int getHeight();
    unsigned int getWidth();
    RenderTarget& operator+=(const RenderTarget& right);

private:
    static const unsigned int DEFAULT_WIDTH = 200;
    static const unsigned int DEFAULT_HEIGHT = 100;

    unsigned int _height;
    unsigned int _width;
    double       _aspectRatio;

    std::vector<std::vector<Vec3>> _targetData;

    void Init(unsigned int width, unsigned int height);
    void writePPM(std::string path);
    void writePNG(std::string path);
    void clear();

    // TODO: Add stuff like clear color here
};
