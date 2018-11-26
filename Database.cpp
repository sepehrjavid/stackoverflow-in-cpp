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