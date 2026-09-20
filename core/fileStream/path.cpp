#include <fileStream/path.h>

namespace path {
    const std::string parse(std::initializer_list<std::string_view> folders)
    {
        std::string path;
        bool first = true;

        for (const auto folder : folders) {
            if (!first)
                path += '\\';

            path += folder;
            first = false;
        }

        return path;
    }
}