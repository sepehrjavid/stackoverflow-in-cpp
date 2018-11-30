#include <iostream>
#include <string>
#include <ctime>
#include "AbstractUser.h"
#include "Exceptions.h"
#include "User.h"
#include "Database.h"
#include "Content.h"

#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif

using namespace std;


enum MenuState {
    START,
    LOGGED_IN,
    MY_CONTENT,
    SEARCH,
    END
};

int main() {
    FILE* datafil = fopen("datfile.txt","r");
    if(!datafil){
        FILE* datafile = fopen("datfile.txt","a");
        fseek(datafile, 0, SEEK_SET);
        fprintf(datafile,"%d", 1);
        fclose(datafile);
        init();
    }
    User::init("SECRET_KEY");
    User * loggedInUser = nullptr;
    MenuState menuState = MenuState::START;
    vector<Content> buff;
    string last_message;
    string user_text;
    int ind = 0;
    char choice;
    while(menuState != MenuState::END) {
        if (menuState != MenuState::MY_CONTENT) {
            system(CLEAR);
        }
        if (!last_message.empty())
            cout << last_message << endl;
        last_message = "";
        switch (menuState) {
            case MenuState::START: {
                cout << "1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch (choice) {
                    case '1': { // login
                        try {
                            string username, password;
                            cout << "Enter Username: ";
                            cin >> username;
                            cout << "Enter Password: ";
                            cin >> password;
                            loggedInUser = &User::login(username,password);
                            loggedInUser->log_user(username);
                            menuState = MenuState::LOGGED_IN;
                        } catch (WrongUsernameOrPasswordException &e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case '2': { // signup
                        try {
                            string username, password, email;
                            cout << "Enter Email: ";
                            cin >> email;
                            cout << "Enter Username: ";
                            cin >> username;
                            cout << "Enter Password: ";
                            cin >> password;
                            loggedInUser = &User::signup(username, password, email);
                            menuState = MenuState::LOGGED_IN;
                            last_message = "User signed up!\n";
                        } catch (UsernameAlreadyExistsException &e) {
                            last_message = e.what();
                            break;

                        } catch (EmailAlreadyExistsException &e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case 'e': { // exit
                        menuState = MenuState::END;
                        break;
                    }
                    default: { // unknown input
                        last_message = "Unknown Input\n";
                        break;
                    }
                }
                break;
            }
            case MenuState::LOGGED_IN: {
                cout << "d.delete account\nl. logout\nq. add question\ns. search a question\nm. my content\ne. exit\n";
                cin >> choice;
                switch (choice) {
                    case 'd': {          //delete account
                        try {
                            loggedInUser->deleteAccount();
                            cout << "Account successfully deleted\n";
                            loggedInUser = nullptr;
                            menuState = MenuState::START;
                        }
                        catch (DeleteAdminException &e) {
                            last_message = e.what();
                        }
                        break;
                    }
                    case 'l': { // logout
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        last_message = "GOOD BYE\n";
                        break;
                    }
                    case 'e': { // exit
                        menuState = MenuState::END;
                        break;
                    }
                    case 'q': { //add question
                        cout<<"Please enter your question:"<<endl;
                        getchar();
                        getline(cin,user_text);
                        loggedInUser->add_question(user_text);
                        last_message = "your question was successfully uploaded\n";
                        break;
                    }
                    case 's': {   //search for question
                        cout<<"search here:"<<endl;
                        getchar();
                        getline(cin, user_text);
                        buff = Content::content_search(user_text);
                        if (buff.size() == 0){
                            cout<<"no match!"<<endl;
                        }
                        else{
                            menuState = MenuState::SEARCH;
                            ind = 0;
                        }
                        break;
                    }
                    case 'm': {
                        buff = Content::my_content(loggedInUser->username);
                        if (buff.size() == 0){
                            cout<<"you have no contents yet"<<endl;
                        }
                        else{
                            menuState = MenuState::MY_CONTENT;
                            ind = 0;
                        }
                        break;
                    }
                    default: { // unknown input
                        last_message = "Unknown Input\n";
                        break;
                    }
                }
                break;
            }
            case MenuState ::MY_CONTENT:{
                cout<<"___________________________________________________________"<<endl;
                buff[ind].print_content();
                cout<<endl<<"e.edit\nd.delete\nn.next\np.previous\nb.back"<<endl;
                cin>>choice;
                switch (choice){
                    case 'n': {
                        if (ind + 1 >= buff.size()){
                            cout<<"no more content after this"<<endl;
                        }
                        else{
                            ind++;
                        }
                        break;
                    }
                    case 'p': {
                        if (ind - 1 < 0){
                            cout<<"no more content prior to this"<<endl;
                        }
                        else{
                            ind--;
                        }
                        break;
                    }
                    case 'd': {

                        break;
                    }
                    case 'e':{

                        break;
                    }
                    case 'b':{
                        menuState = MenuState::LOGGED_IN;
                        ind = 0;
                        buff.clear();
                        break;
                    }
                }
                break;
            }
            case MenuState::SEARCH:{
                cout<<"___________________________________________________________"<<endl;
                buff[ind].print_content();
                cout<<endl<<"r.reply\nn.next\np.previous\nb.back"<<endl;
                cin>>choice;
                switch (choice){
                    case ('n'):{
                        if (ind + 1 >= buff.size()){
                            cout<<"no more content after this"<<endl;
                        }
                        else{
                            ind++;
                        }
                        break;
                    }
                    case ('p'):{
                        if (ind - 1 < 0){
                            cout<<"no more content prior to this"<<endl;
                        }
                        else{
                            ind--;
                        }
                        break;
                    }
                    case ('r'):{

                        break;
                    }
                    case ('b'):{
                        menuState = MenuState::LOGGED_IN;
                        ind = 0;
                        buff.clear();
                        break;
                    }
                }
                break;
            }
        }
    }
    system(CLEAR);
    cout << "GOODBYE" << endl;
    return 0;
}
