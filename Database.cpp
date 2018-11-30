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
        user_id = query_user(username);
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

int query_user(std::string username, std::string email){
    int count;
    if (email == ""){
        db << "select count(*) from User where username = ?;"<< username >> count;
        if (count == 0){
            return count;
        }
        db << "select _id from User where username = ?;"
           << username
           >> count;
    }
    if (username == ""){
        db << "select count(*) from User where email = ?;"<< email >> count;
        if (count == 0){
            return count;
        }
        db << "select _id from User where email = ?;"
           << email
           >> count;
    }
    return  count;
}


int Create_CR(Content* destination, Content* source ,ContentRelationType type){
    int a1,a2;
    string tt;
    if(type==ContentRelationType::ANSWER_TO){tt="ANSWER_TO";}
    if(type==ContentRelationType::DUPLICTE_OF){tt="DUPLICTE_OF";}
    db << "select _id from Content where body=? and visits=?  ;"
       << source->body << source->visits
       >> [&](int _id) { a1 = _id; };
    db << "select _id from Content where body=? and visits=?  ;"
       <<destination->body << destination->visits
       >> [&](int _id) { a2 = _id; };
    db << "insert into ContentRelation (type,destination_id,source_id) values (?,?,?);"
       << tt
       << a2
       << a1;
    return db.last_insert_rowid();
}
int Delete_CR(Content* destination, Content* source ,ContentRelationType type){
    int a1,a2;
    string tt;
    if(type==ContentRelationType::ANSWER_TO){tt="ANSWER_TO";}
    if(type==ContentRelationType::DUPLICTE_OF){tt="DUPLICTE_OF";}
    db << "select _id from Content where body=? and visits=?  ;"
       << source->body << source->visits
       >> [&](int _id) { a1 = _id; };
    db << "select _id from Content where body=? and visits=?  ;"
       <<destination->body << destination->visits
       >> [&](int _id) { a2 = _id; };
    db << "DELETE from ContentRelation where type = ? and destination_id = ? and source_id = ?;"
        << tt
        << a2
        << a1;
    return 0;
}


