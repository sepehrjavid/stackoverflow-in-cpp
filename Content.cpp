#include "Content.h"
#include <iostream>
#include <vector>

#include <sqlite_modern_cpp.h>
using namespace sqlite;


using namespace std;

Content::Content(std::string body, ContentType type):body(body), type(type){
    this->visits = 0;
}

Content::~Content() {
    int end = relations.size();
    for (int i = 0; i < end;i++){
        delete relations[0];
    }
}

void Content::add_relation(ContentRelationType type, Content &dest){
    ContentRelation* cp = new ContentRelation(&dest, this,  type);
    relations.push_back(cp);
    dest.relations.push_back(cp);
}


void Content::edit_content(std::string body) {
    this->body = body;
}

void Content::print_answers() {
    for (int i=0;i<relations.size();i++){
        if (relations[i]->type == ANSWER_TO){
            cout<<i+1<<". "<<relations[i]->source->body<<endl;
        }
    }
    cout<<"\n\n";
}

ContentRelation::ContentRelation(Content* destination, Content* source ,ContentRelationType type) {
    this->destination = destination;
    this->source = source;
    this->type = type;
    string tt;
    if(type==ContentRelationType::ANSWER_TO){tt="ANSWER_TO";}
    if(type==ContentRelationType::DUPLICTE_OF){tt="DUPLICTE_OF";}
    int a1,a2;
    database db("dbfile.db");
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
}
ContentRelation::~ContentRelation() {
    vector<ContentRelation*> *c1 = &(this->destination->relations);
    for (auto i = c1->begin(); i < c1->end(); i++) {
        if (*i == this) {
            c1->erase(i);
        }
    }
    c1 = &(this->source->relations);
    for (auto i = c1->begin(); i < c1->end(); i++) {
        if (*i == this) {
            c1->erase(i);
        }
    }
//    database db("dbfile.db");
//    db << "delete _id from user where body=? and visits=?  ;"
//       << source->body << source->visits

}