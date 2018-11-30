#include <utility>

#include <sstream>
#include "User.h"
#include "Exceptions.h"
#include <iostream>
#include <vector>
#include "Content.h"
#include "Logger.h"
#include "Database.h"

vector<User> User::users;
string User::salt;

extern sqlite::database db;

User::User(string username, string password, string email, UserType type){
//    lower(username);
    this->username = username;
    set_password(std::move(password));
    this->email = email;
    this->type = type;
}

void User::set_password(string password){
    size_t ps = pass_hash(password + salt);
    stringstream out;
    out << ps;
    this->password = out.str();
}

bool User::check_password(string password){
    size_t check = pass_hash(password + salt);
    stringstream out;
    out << check;
    return (this->password == out.str());
}

bool User::authenticate(string username, string password){
    int id = query_user(username);
    if (id == 0){
        return false;
    }
    std::string pass, email1;
    db << "select hash_pass, email from User where _id = ?;"<<id>>tie(pass, email1);
    User* usr = new User(username, " ",email1, UserType::MEMBER);
    usr->password = pass;
    return usr->check_password(password);
}
void User::deleteAccount(){
    if (this->type == UserType::ADMIN) {
        throw DeleteAdminException();
    }

    for (auto user = users.begin(); user != users.end();user++){
        if ( user->username == this->username  ) {
            users.erase(user);
            break;
        }
    }
}

User& User::login(string username, string password){
    if(authenticate(username, password)) {
        std::string email1, type;
        db << "select email, type from User where username = ?;"<<username>>tie(email1, type);
        if (type == "MEMBER"){
            return *(new User(username, password, email1, UserType::MEMBER));
        }
        else{
            return *(new User(username, password, email1, UserType::ADMIN));
        }
    }
    throw WrongUsernameOrPasswordException();
}

void User::log_user(string username){
    char llg[100];
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S\n",timeinfo);
    std::string strr(buffer);
    const char *usg = username.c_str();
    const char *emg = this->email.c_str();
    const char *utg = strr.c_str();
    sprintf(llg,"%s %s %s",emg,usg,utg);
    _Log(llg);
}

User& User::signup(string username, string password, string email){
    if (query_user(username) != 0) {
        throw UsernameAlreadyExistsException();
    }
    else if (query_user("", email) != 0) {
        throw EmailAlreadyExistsException();
    }
    users.emplace_back(username, password, email, UserType::MEMBER);
    hash<string> temp;
    size_t ps = temp(password + salt);
    stringstream out;
    out << ps;
    create_user(username, email, "MEMBER", out.str());
    return users[users.size() - 1];
}

void User::init(const string &salt) {
    User::salt = salt;
    users.reserve(20);
    users.emplace_back("admin", "admin", "admin@stackoverflow.com", UserType::ADMIN);
}


void User::add_question(string body){
    Content* cp = new Content(body, QUESTION);
    contents.push_back(cp);
    create_content(body , "QUESTION", username);
}

void User::add_answer(string body, Content &q){
    Content* cp = new Content(body, ANSWER);
    contents.push_back(cp);
    cp->add_relation(ANSWER_TO, q);
}
void User::remove_content(Content &target) {
    for (auto it = contents.begin(); it != contents.end();it++){
        if (*it == &target){
            contents.erase(it);
            break;
        }
    }
    delete &target;
}

void User::make_content_dup(Content &source, Content &target) {
    if (type == ADMIN){
        ContentRelation* instance = new ContentRelation(&target, &source, DUPLICTE_OF);
        source.relations.push_back(instance);
        target.relations.push_back(instance);
    }
    else{
        NotAdminForDuplicateEception ex;
        throw ex;
    }
}


void User::get_my_content(int ind) {
    cout<<contents[ind]->body<<endl;
    cout<<"visits: "<<contents[ind]->visits<<endl;
    if (contents[ind]->type == QUESTION){
        cout<<"------------------------answers--------------------------"<<endl;
        contents[ind]->print_answers();
    }
}

fstream& User::to_string(fstream &out){
    out << this->email << " " << this->username << endl;
    return out;
}
fstream& operator << (fstream &out,User a){
    return a.to_string(out);
}