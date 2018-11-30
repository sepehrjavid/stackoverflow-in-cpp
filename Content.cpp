#include "Content.h"
#include <iostream>
#include <vector>
#include "Database.h"




using namespace std;


Content::Content(std::string body, ContentType type, int visits):body(body), type(type), visits(visits){}

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


void Content::edit_content(std::string bodyy) {
    Edit_content(this->body, this->visits, this->type , bodyy);
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


std::vector<Content> Content::content_search(std::string query){
    vector<Content> out;
    query_content(query, out);
    return out;
}


void Content::print_content(){
    if (type == ContentType::QUESTION){
        update_content(body, visits+1);
        cout<<body<<endl<<endl<<"visits = "<<visits<<endl;
        vector<Content> ans;
        int pk = query_content(body);
        get_answers(pk, ans);
        cout << "-------------------answers-----------------------"<<endl;
        if (ans.size() == 0){
            cout<<"No answers for this question yet"<<endl;
            return;
        }
        for(int i = 0; i<ans.size();i++){
            ans[i].print_content();
        }
    }
    else if (type == ContentType::ANSWER){
        update_content(body, visits+1);
        cout<<body<<endl<<endl<<"visits = "<<visits<<endl
        <<"-------------------------------------------------"<<endl;
    }
}

//ContentRelation::ContentRelation(Content* destination, Content* source ,ContentRelationType type)
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