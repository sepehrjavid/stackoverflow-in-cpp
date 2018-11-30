#include "Content.h"
#include <iostream>
#include <vector>

#include "Database.h"



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

ContentRelation::ContentRelation(Content* destination, Content* source ,ContentRelationType typee) {
    this->destination = destination;
    this->source = source;
    this->type = typee;
    Create_CR(destination, source, typee);
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
    Delete_CR(this->destination,this->source,this->type);

}