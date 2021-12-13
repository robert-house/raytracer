#include "MaterialFactory.h"

// TODO: Remove when log is singleton
MaterialFactory::MaterialFactory() {}

MaterialFactory::MaterialFactory(Log* log)
{
    _pLog = log;
}

MaterialFactory::~MaterialFactory()
{
}

Material* MaterialFactory::MakeMaterial(std::deque<std::string>& materialDescription)
{
    Material* pMaterial = nullptr;
    std::string materialName = materialDescription.front();
    materialDescription.pop_front();

    double albedoX, albedoY, albedoZ, roughness, refractIndex = 0.0;

    // Decide what material to make
    if (materialName.compare("LAMBERTIAN") == 0)
    {
        // Assign Properties
        albedoX = stod(materialDescription.front());
        materialDescription.pop_front();
        albedoY = stod(materialDescription.front());
        materialDescription.pop_front();
        albedoZ = stod(materialDescription.front());
        materialDescription.pop_front();

        // Create Material
        pMaterial = new Lambertian(albedoX, albedoY, albedoZ);
    }
    else if (materialName.compare("METAL") == 0)
    {
        // Assign Properties
        albedoX = stod(materialDescription.front());
        materialDescription.pop_front();
        albedoY = stod(materialDescription.front());
        materialDescription.pop_front();
        albedoZ = stod(materialDescription.front());
        materialDescription.pop_front();
        roughness = stod(materialDescription.front());
        materialDescription.pop_front();

        // Create Material
        pMaterial = new Metal(albedoX, albedoY, albedoZ, roughness);
    }
    else if (materialName.compare("DIELECTRIC") == 0)
    {
        // Assign Properties
        refractIndex = stod(materialDescription.front());
        materialDescription.pop_front();

        // Create Material
        pMaterial = new Dielectric(refractIndex);
    }
    else if (materialName.compare("DIFFUSE_EMITTER") == 0)
    {
        // Assign Properties
        albedoX = stod(materialDescription.front());
        materialDescription.pop_front();
        albedoY = stod(materialDescription.front());
        materialDescription.pop_front();
        albedoZ = stod(materialDescription.front());
        materialDescription.pop_front();

        pMaterial = new DiffuseEmitter(albedoX, albedoY, albedoZ);
    }
    else
    {
        // Missing material. Report error
        _pLog->error("Missing material detected");

        // Assign Default MISSING_MATERIAL type
        pMaterial = new MissingMat();
    }

    return pMaterial;
}

Material* MaterialFactory::GetRandomMaterial()
{
    double choose_mat = GetRandom();

    if (choose_mat < 0.8)
    {
        // Diffuse
        return new Lambertian(GetRandom() * GetRandom(), GetRandom() * GetRandom(), GetRandom() * GetRandom());

    }
    else if (choose_mat < 0.95)
    {
        // Metal
        return new Metal(0.5 * (1 + GetRandom()), 0.5 * (1 + GetRandom()), 0.5 * (1 + GetRandom()), 0.5 * GetRandom());
    }
    else
    {
        // Glass
        return new Dielectric(1.5);
    }
}

// TODO: Create Random Library
double MaterialFactory::GetRandom()
{
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}
