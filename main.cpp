#include "GameController.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// CMake copies the Assets directory beside the executable.
const string assetDirectory = "./Assets";

class GameWorld;

GameWorld* createStudentWorld(string assetPath = "");

int main(int argc, char* argv[])
{
    filesystem::path assetPath(assetDirectory);
    if (!assetPath.empty())
    {
        error_code ec;
        if (! filesystem::is_directory(assetPath, ec))
        {
            cout << "Cannot find directory " << assetPath << endl;
            return 1;
        }
    }
    {
        const string someAsset = "spring.png";
        ifstream ifs(assetPath / someAsset);
        if (!ifs)
        {
            cout << "Cannot find " << someAsset << " in ";
            cout << (assetDirectory.empty() ? "current directory" : assetDirectory) << endl;
            cout << "See the comment at the top of main.cpp" << endl;
            return 1;
        }
    }

    GameWorld* gw = createStudentWorld(assetPath.string());
    Game().run(argc, argv, gw, "Lemmings");
}
