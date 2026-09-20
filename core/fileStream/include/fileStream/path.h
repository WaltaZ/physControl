#pragma once

#include <string>

namespace path {

    const std::string CACHE_DIR = "cache";

    const std::string parse(std::initializer_list<std::string_view> folders);

}