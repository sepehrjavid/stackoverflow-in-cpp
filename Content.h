#include<iostream>
#include<vector>
#pragma once

enum ContentType{
    QUESTION,
    ANSWER
};
enum ContentRelationType{
    DUPLICTE_OF,
    ANSWER_TO
};
class Content;

class ContentRelation{
public:
    ContentRelationType type;
    Content* destination ;
    Content* source ;
    ContentRelation(Content* destination, Content* source , ContentRelationType  type);
    ~ContentRelation();
};

class Content{
public:
    std::string body;
    ContentType type;
    int visits;
    std::vector<ContentRelation*> relations;
    Content(std::string body, ContentType type);
    ~Content();
    void add_relation(ContentRelationType type, Content &dest);
    void edit_content(std::string body);
    static std::vector<Content> search(std::string query);
    void print_answers();
};