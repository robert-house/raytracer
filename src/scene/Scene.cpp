#include "Scene.h"

// TODO: Remove log when it is a singleton
Scene::Scene(Log* log)
{
    _entities = std::vector<Shape*>();
    _shapeFactory = ShapeFactory(log);
    _pLog = log;
}
Scene::~Scene() {}

void Scene::AddToScene(Shape* shape)
{
    _entities.push_back(shape);
}

// TODO: Implement BVH
// FIXME: Huge performance impact (42% runtime). A better solution would be to implement a BVH
bool Scene::TestForCollision(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord)
{
    HitRecord tempHitRecord;
    bool collision = false;
    double closestObject = tMax;

    // Iterate through the list
    for (auto entity : _entities)
    {
        if (entity->hit(ray, tMin, closestObject, tempHitRecord))
        {
            collision = true; // TODO: Why not just return on the first object
            closestObject = tempHitRecord.t;
            hitRecord = tempHitRecord;
        }
    }

    return collision;
}

// TODO: Error handling
bool Scene::LoadFromFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;
    std::string token;

    std::deque<std::string> tokens;
    Shape* shape = nullptr;

    _pLog->openSection("[SCENE] Opening Scene From " + fileName + "...");

    // Do Parsing Here
    // in the form SHAPE X Y Z SIZE MATERIAL aX aY aZ SPECIAL_PARAMS
    if (file.is_open())
    {
        // While there still are lines to read, take each line
        // and pass it to a shape factory to create the shape
        // specified for each line. ONLY a shape object will
        // be passed back
        while (std::getline(file, line, '\n'))
        {
            // Seperate into tokens
            std::stringstream ss(line);
            while (std::getline(ss, token, ' '))
            {
                tokens.push_back(token);
                // TODO: If debug enabled, print to console
            }
            // Pass token list to ShapeFactory
            shape = _shapeFactory.MakeShape(tokens);
            _entities.push_back(shape);

            _pLog->debug(shape->toString());

            // When done, clear vector
            tokens.clear();
        }
    }

    _pLog->closeSection("Done");

    return true;
}

void Scene::GenerateRandomScene()
{
    Shape* pShape = nullptr;

    for (int a = -10; a < 10; a++)
    {
        for (int b = -10; b < 10; b++)
        {
            pShape = _shapeFactory.GetRandomShape(a, b);

            if (pShape != nullptr)
            {
                _entities.push_back(pShape);
            }

            pShape = nullptr;
        }
    }
}

// TODO: revisit. May not make sense to also load new scene
void Scene::resetScene(std::string filePath)
{
    bool status = false;

    _entities.clear();

    if (filePath != "")
    {
        status = LoadFromFile(filePath);
    }

    if (!status)
    {
        _pLog->error("Scene was reset, but an error occured when loading new scene");
    }
}