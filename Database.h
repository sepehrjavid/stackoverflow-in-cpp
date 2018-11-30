#pragma once

#include "User.h"
#include "Content.h"

#include <sqlite_modern_cpp.h>

void init();

int create_content(std::string body, std::string type, std::string username, std::string reply = "");

int create_user(std::string username, std::string email, std::string type, std::string hash_pass);

int query_user(std::string username, std::string email = "");

void delete_user(std::string username);

void query_content(std::string body, vector<Content>& fill);

int query_content(std::string body);

void update_content(std::string body, int new_visit);

void get_answers(int id, vector<Content>& out);

int Create_CR(Content* destination, Content* source ,ContentRelationType type);

int Delete_CR(Content* destination, Content* source ,ContentRelationType type);

