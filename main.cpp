#include "GameController.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

  // If your program is having trouble finding the Assets directory,
  // replace the string literal with a full path name to the directory,
  // e.g., "C:/CS32/Lemmings/Assets" or "/Users/fred/cs32/Lemmings/Assets"

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
