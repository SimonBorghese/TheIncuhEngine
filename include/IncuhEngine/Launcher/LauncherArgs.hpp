#ifndef LAUNCHERARGS_HPP
#define LAUNCHERARGS_HPP
#include <cstring>
#include <nlohmann/json.hpp>
#include <fstream>
#include <fmt/format.h>

namespace LArgs{

    typedef struct{
        std::string title;
        uint32_t width;
        uint32_t height;
        std::string tMap;
        float FOV;
        float brightness;
    } arguments;

    class LauncherArgs
    {
        public:
            LauncherArgs(const char *targetFile, int argc, char **args);
            virtual ~LauncherArgs();

            arguments* getArguments(){
                return lArgs;
            }
        protected:

        private:
            arguments *lArgs;
    };
}

#endif // LAUNCHERARGS_HPP
