//-------------------------------------------------------------------------//
// [FILE NAME]                                                             //
// [DESCRIPTION]                                                           //
// Author:  Robert House [DATE]                                            //
//-------------------------------------------------------------------------//
// [CLASS NAME]: [SUMMATION OF CLASS FUNCTIONALITY]                        //
//                                                                         //
//	 -- [HIGHLIGHT OF FUNCTIONALITY 1]                                     //
//	 -- [HIGHLIGHT OF FUNCTIONALITY 2]                                     //
//	 -- [HIGHLIGHT OF FUNCTIONALITY 3]                                     //
//                                                                         //
// Assumptions:                                                            //
//   -- >> [ASSUMPTION 1]                                                  //
//-------------------------------------------------------------------------//

// In this particular instance, a render target is just going to be a file stream
// that will output a *.ppm file
#include <iostream>
#include <iomanip>
#include "./renderer/RayTracer.h"
#include "./scene/Scene.h"
#include "logging/Log.h"
#include "logging/ConsoleLog.h"
#include "config/Config.h"

// TODO:

Scene* pScene;
Camera* pCamera;
RayTracer* pTracer;
std::shared_ptr<Log> pLog(new ConsoleLog(Log::INFO, false));

static std::string VERSION = "0.1.202002";

void init(std::string fileName, std::string configFile);
void mainLoop();
void oldLoop();
void printHeader();

int main(int argc,  const char* argv[])
{
    std::string configFile;
    std::string fileName;

    std::shared_ptr<spdlog::logger> log = spdlog::stdout_color_st("console");

    // Enable verbose mode
    pLog->setVerbosity(true);

    // cornellbox.txt 256 256 128 8 278 278 -800 8
    if (argc == 2)
    {
        configFile = argv[1];
    }
    else
    {
        configFile = "../config.cfg";
    }

    fileName = "../cornellBox.txt";
    //pWindow = new Window();

    // TODO arg for whether we want sdl2 or cli
    init(fileName, configFile); // This being commented out was creating the segfault. Singleton delete error?

    oldLoop();
    //mainLoop();

    // Do cleanup
    delete pScene;
    delete pCamera;
    delete pTracer;

    pLog.reset();

    // Explicit call to singleton destructor? didnt fix
    Config::getInstance().~Config();

    return 0;
}

void init(std::string fileName, std::string configFile)
{
    printHeader();

    // Init Logger
    pLog->log(Log::INFO, "Log Has Started");

    // Attach Logger to config
    Config::getInstance().attachLogger(pLog);

    // Setup the Scene
    pScene = new Scene(pLog.get());
    pScene->LoadFromFile(fileName);
    //scene.GenerateRandomScene();

    // Setup Camera
    glm::vec3 cameraAt(278, 278, -800);
    glm::vec3 lookAt(278, 278, 0);
    double dist_to_focus = (cameraAt - lookAt).length();
    double aperture = 0.1;

    pTracer = new RayTracer(pLog);

    // TODO: Come up with a better solution to aspect ratio calculations and settings
    double width = Config::getInstance().getCVar("r_width")->getValue();
    double height = Config::getInstance().getCVar("r_height")->getValue();
    pCamera = new Camera(cameraAt, lookAt, glm::vec3(0, 1, 0), 40, 1.0 / 1.0, aperture, dist_to_focus);
    pCamera->UpdateCameraPosition(287, 287, -800);

    // TODO: Investigate calling conventions for pointers. If the camera object pointer was
    //       registered before the creation of the object, it will return nullptr.
    //       Perhaps modify the register functions to throw an error if its nullptr?
    pTracer->registerScene(pScene);
    pTracer->registerCamera(pCamera);

    // Cannot decide whether I want to go with the paradigm of setState->render() or render(state);
    //pTracer->render();


    //Config::getInstance().print();

    // Pointer to function test
    // Could store pointer to Component
    // When command is assigned to a CVar, pass &Component::reinit()
    // UGH, this means we would have to store the pointer to the component IN the CVAR.
    // This is a BAD idea
    //void (RayTracer::*funcPtr)();
    //funcPtr = &RayTracer::Update;
    //(tracer.*funcPtr)();

    // A better way to handle this would be to store the local state of certain variables that
    // we know to require a reinit and do a reinit on the next update call. This would not solve
    // the problem of being able to issue commands though...more thought is needed before I choose
    // a path here.

    // Load config file
    Config::getInstance().loadConfigFile(configFile);
}

void oldLoop()
{
    bool running = true;

    std::string buffer;

    while (running)
    {
        // Take in command from shell
        std::cout << ">> ";
        std::getline(std::cin, buffer);

        if (buffer == "exit" || buffer == "quit" || buffer == "bye" || buffer == "q")
        {
            running = false;
        }
        else if (buffer == "print")
        {
            Config::getInstance().print();
        }
        else if(buffer == "render" || buffer == "r")
        {
            pTracer->render();
        }
        else if (buffer == "load")
        {
            // TODO: more elegant solution for this
            std::cout << ">> Please enter fileName: ";
            std::getline(std::cin, buffer);
            pScene->resetScene(buffer);
        }
        else if (buffer == "load config")
        {
            std::cout << ">> Please Enter Config Filename: ";
            std::getline(std::cin, buffer);
            Config::getInstance().loadConfigFile(buffer);
        }
        else if (buffer == "help" || buffer == "h")
        {
            std::cout
            << "[help | h]   -- Display this menu" << std::endl
            << "[cvar]       -- See command description" << std::endl
            << "[cvar value] -- Set command to value" << std::endl
            << "[render | r] -- Start rendering pass" << std::endl
            << "[quit | q]   -- Quit program" << std::endl;
        }
        else
        {
            Config::getInstance().parseInput(buffer);
        }

        buffer = "";
    }
}

void printHeader()
{
    std::cout
    << "======================= RayTracer =======================" << std::endl
    << "Robert House" << std::endl
    << "©2020" << std::endl
    << "Version: "  << VERSION << "⍺" << std::endl
    << std::endl
    << "Type 'h' or 'help' to see options" << std::endl
    << "=========================================================" << std::endl;
}