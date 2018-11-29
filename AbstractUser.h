#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include "Content.h"

//#define lower(str) transform(str.begin(), str.end(), str.begin(), ::tolower)
using namespace std;

enum UserType {
    ADMIN,
    MEMBER
};

class AbstractUser{
public:
    hash<string> pass_hash;
    virtual void deleteAccount() = 0;
    string username;
    vector<Content*> contents;      //content pointer used to make sure that if the content is edited by a method in content class, the content in the vector is also edited
protected:
    string email;
    string password;
    UserType type;
};
