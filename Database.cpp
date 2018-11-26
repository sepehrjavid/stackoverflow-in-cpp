//
// Created by navid-fkh on 11/23/18.
//
#include "User.h"
#include "Content.h"


#include "sqlite_orm.h"
using namespace sqlite_orm;

void init() {
    auto storage = make_storage("db.sqlite",
                                make_table("USER",
                                           make_column("pass_hash", &User::pass_hash,default_value(0)),
                                           make_column("username", &User::username)
//                                           make_column("email", &User::email),
//                                           make_column("type", &User::type)
                                ),
                                make_table("Content",
                                           make_column("body", &Content::body),
                                           make_column("type", &Content::type),
                                           make_column("visits", &Content::visits)
                                ),
                                make_table("ContentRelation",
//                                           make_column("body", &ContentRelation::source),
                                           make_column("type", &ContentRelation::type)
//                                           make_column("destination", &ContentRelation::destination)
                                ));
    storage.sync_schema();
    string uname = string("navid");
    string upass = string("123");
    string uemail = string("navid@gmail.com");
    User Navid{
        uname,
        upass,
        uemail,
        UserType::ADMIN
    };
    storage.insert(Navid);
}
