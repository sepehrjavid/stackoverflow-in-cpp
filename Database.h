#pragma once

#include "User.h"
#include "Content.h"

#include <sqlite_modern_cpp.h>

void init();

int create_content(std::string body, std::string type, std::string username, std::string reply = "");

int create_user(std::string username, std::string email, std::string type, std::string hash_pass);
