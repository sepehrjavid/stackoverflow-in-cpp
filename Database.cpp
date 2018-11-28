#include "Database.h"
using namespace sqlite;

database db("dbfile.db");
void init(){
    db << "create table User ("
          "_id integer primary key autoincrement not null,"
          "username text,"
          "email text,"
          "hash_pass text,"
          "type text"
          ");";
    db << "create table Content ("
          "_id integer primary key autoincrement not null,"
          "body text,"
          "user_id integer,"
          "type text,"
          "visits integer"
          ");";
    db << "create table ContentRelation ("
          "source_id integer,"
          "destination_id integer,"
          "type text"
          ");";
}


int create_content(std::string body, std::string type, std::string username, std::string reply){
    int user_id;
    if (type  == "QUESTION"){
        db << "select _id from User where username = ?;"
            << username
            >> user_id;
        cout << user_id <<endl;
        db << "insert into content (body, type, visits, user_id) values (?,?,?,?);"
           << body
           << type
           << 0
           << user_id;
    } else if (type == "ANSWER"){

    }
    return db.last_insert_rowid();
}

int create_user(std::string username, std::string email, std::string type, std::string hash_pass){
    db << "insert into User (username, email, type, hash_pass) values (?,?,?,?);"
        << username
        << email
        << type
        << hash_pass;
    return db.last_insert_rowid();
}




