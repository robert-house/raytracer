#include "ShapeFactory.h"

ShapeFactory::ShapeFactory() {} // TODO: Handle Default Constructor
ShapeFactory::ShapeFactory(Log* log)
{
    matFac = MaterialFactory(log);
    _pLog = log;
}
ShapeFactory::~ShapeFactory() {}

Shape* ShapeFactory::MakeShape(std::deque<std::string>& shapeDescription)
{
    std::string shape;
    Material* pMaterial = nullptr;
    Shape* pShape = nullptr;
    double posX, posY, posZ, size;
    // TODO: Check to see if the deque is empty first

    // Get first element to decide what shape it is
    shape = shapeDescription.front();
    shapeDescription.pop_front();

    // Figure out what shape to make
    if (shape.compare("SPHERE") == 0)
    {
        // Read in the position and size
        posX = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        posY = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        posZ = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        size = std::stod(shapeDescription.front());
        shapeDescription.pop_front();

        // It is a sphere
        pShape = new Sphere(glm::vec3(posX, posY, posZ), size, nullptr);
    }
    else if (shape.compare("RECT") == 0)
    {
        std::string rectangleType;
        Rect::RectType type;
        double x, y, z, length, width, normalDirection;

        rectangleType = shapeDescription.front();
        shapeDescription.pop_front();

        // Do check for rectangle type here
        if (rectangleType.compare("XY") == 0)
        {
            type = Rect::RectType::XY;
        }
        else if (rectangleType.compare("XZ") == 0)
        {
            type = Rect::RectType::XZ;
        }
        else
        {
            type = Rect::RectType::YZ;
        }

        x = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        y = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        z = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        length = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        width = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        normalDirection = std::stod(shapeDescription.front());
        shapeDescription.pop_front();

        pShape = new Rect(type, glm::vec3(x,y,z), length, width, normalDirection, nullptr);
    }
    else if (shape.compare("TRIANGLE") == 0)
    {
        double v0x, v0y, v0z, v1x, v1y, v1z, v2x, v2y, v2z;

        // Read in the position and size
        v0x = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        v0y = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        v0z = std::stod(shapeDescription.front());
        shapeDescription.pop_front();

        v1x = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        v1y = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        v1z = std::stod(shapeDescription.front());
        shapeDescription.pop_front();

        v2x = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        v2y = std::stod(shapeDescription.front());
        shapeDescription.pop_front();
        v2z = std::stod(shapeDescription.front());
        shapeDescription.pop_front();

        size = std::stod(shapeDescription.front());
        shapeDescription.pop_front();

        pShape = new Triangle(glm::vec3(v0x, v0y, v0z), glm::vec3(v1x, v1y, v1z), glm::vec3(v2x, v2y, v2z), size, nullptr);
    }

    // Generate and assign the material
    pMaterial = matFac.MakeMaterial(shapeDescription);

    // Assign
    pShape->UpdateMaterial(pMaterial);

    return pShape;
}

Shape* ShapeFactory::GetRandomShape(int seedA, int seedB)
{
    glm::vec3 center(seedA + 0.9 * GetRandom(), 0.2, seedB + 0.9 * GetRandom());
    glm::vec3 temp = center - glm::vec3(4, 0.0, 0.0);
    double length = temp.length();

    if (length > 0.9)
    {
        return new Sphere(center, 0.2, matFac.GetRandomMaterial());
    }
    else
    {
        return nullptr;
    }
}

// TODO: Create Random Library
double ShapeFactory::GetRandom()
{
    return (static_cast <double> (rand()) / static_cast <double> (RAND_MAX));
}