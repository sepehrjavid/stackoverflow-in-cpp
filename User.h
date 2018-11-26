#pragma once

#include <iostream>
#include "AbstractUser.h"
#include "Content.h"

class User : public AbstractUser {
public:
    friend fstream& operator << (fstream,User a);
    User(string username, string password, string email, UserType type);

    static void init(const string &salt);

public:
    void set_password(string password);
    fstream& to_string(fstream& out);
    bool check_password(string password);

public:
    bool authenticate(string username, string password);
    void deleteAccount();
    void add_question(string body);
    void add_answer(string body, Content &question);
    void remove_content(Content &target);
    void make_content_dup(Content &source, Content &target);
    void get_my_content(int ind);
    void log_user(string username);
public:
    static User& login(string username, string password);
    static User& signup(string username, string password, string email);
private:
    static string salt;
    static vector<User> users;

};
