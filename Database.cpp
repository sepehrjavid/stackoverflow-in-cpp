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
        db << "insert into Content (body, type, visits, user_id) values (?,?,?,?);"
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

int query_user(std::string username, std::string email){                    //returns 0 if not found
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
    return count;
}


void delete_user(std::string username){
    db << "delete from User where username = ?;"<<username;
}


void query_content(std::string text, vector<Content>& fill){
//    std::string text, type;
//    int visits;
    try{
        db << "select visits, type, body from Content where body like ?;"
           << "%"+text+"%"
           >> [&] (int visits, std::string type, std::string body){
               if (type == "QUESTION"){
                   fill.emplace_back(body, ContentType::QUESTION, visits);
               }
               else{
                   fill.emplace_back(body, ContentType::ANSWER, visits);
               }
           };
    }
    catch (exception& e){
        cout << e.what() << endl;
    }
}

int query_content(std::string body){
    int id;
    cout<<body<<"2"<<endl;
    try{
        db << "select _id form Content where body = ?;"
           <<body
           >>id;
    }
    catch (exception& e){
        cout << e.what() << endl;
    }
    return id;
}

void update_content(std::string body, int new_visit){
    db << "update Content set visits = ? where body = ?;"
    <<new_visit
    <<body;
}


void get_answers(int id, vector<Content>& out){
    vector<int> temp;
    try{
        db << "select source_id from ContentRelation where destination_id = ? and type = 'ANSWER_TO';"
           <<id
           >> [&](int source_id){
               temp.push_back(source_id);
           };
        for (int it = 0; it < temp.size(); it++){
            try{
                db << "select visits, type, body from Content where _id = ?;"
                   << temp[it]
                   >> [&] (int visits, std::string type, std::string body){
                       if (type == "QUESTION"){
                           out.emplace_back(body, ContentType::QUESTION, visits);
                       }
                       else{
                           out.emplace_back(body, ContentType::ANSWER, visits);
                       }
                   };
            }
            catch (exception& e){
                cout << e.what() << endl;
            }
        }
    }
    catch (exception& e){
        cout << e.what() << endl;
    }
}



