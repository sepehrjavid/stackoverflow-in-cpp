#pragma once

#include "User.h"
#include "Content.h"

#include <sqlite_modern_cpp.h>

void init();

void create_content(std::string body, std::string type, std::string username, std::string reply = "");
