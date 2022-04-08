#include <Launcher/LauncherArgs.hpp>

LArgs::LauncherArgs::LauncherArgs(const char *targetFile, int argc, char **args)
{
    lArgs = new arguments;
    lArgs->width = 0;
    lArgs->height = 0;
    lArgs->tMap = "";
    for (int a = 1; a < argc && (a+1) < argc; a++){
        if (!strcmp(args[a], "-width")){
            lArgs->width = atoi(args[++a]);
        }
        else if (!strcmp(args[a], "-height")){
            lArgs->height = atoi(args[++a]);
        }
        else if (!strcmp(args[a], "-map")){
            lArgs->tMap = std::string(args[++a]);
        }
        else{
            printf("Unidentified argument: %s\n", args[a]);
            ++a;
        }
    }
    std::ifstream i(targetFile);

    nlohmann::json j;
    i >> j;

    lArgs->title = j["title"];
    lArgs->FOV = j["fov"];
    lArgs->brightness = j["brightness"];

    if (!lArgs->width){
        lArgs->width = j["width"];
    }
    if (!lArgs->height){
        lArgs->height = j["height"];
    }

    if (!strcmp("", lArgs->tMap.c_str())){
        lArgs->tMap = fmt::format("{}", j["map"]);
    }

}

LArgs::LauncherArgs::~LauncherArgs()
{
    free(lArgs);
}
