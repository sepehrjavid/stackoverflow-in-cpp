#include <iostream>
#include <string>
#include <ctime>
#include "AbstractUser.h"
#include "Exceptions.h"
#include "User.h"
#include "Database.h"

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
    END
};

int main() {
//    init();
    User::init("SECRET_KEY");
    User * loggedInUser = nullptr;
    MenuState menuState = MenuState::START;
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
                    case 'd': {
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
                        //getline(cin,user_text);                           //TODO find a better way to get a line including spaces
                        cin>>user_text;
                        loggedInUser->add_question(user_text);
                        last_message = "your question was successfully uploaded\n";
                        break;
                    }
                    case 's': {
                        cout<<"search here:"<<endl;
                        cin>> user_text;
                        // a search
                        break;
                    }
                    case 'm': {
                        if (loggedInUser->contents.size() == 0){
                            last_message = "you don't have any contents yet\n";
                        }
                        else {
                            system(CLEAR);
                            loggedInUser->get_my_content(ind);
                            menuState = MenuState::MY_CONTENT;
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
                cout<<"n. next\np. previous\nd. delete content\ne. edit content\nb. back"<<endl;
                cin>>choice;
                system(CLEAR);
                switch (choice){
                    case 'n': {
                        ind++;
                        if (ind == loggedInUser->contents.size()){
                            ind--;
                            last_message = "no contents after this";
                        }
                        loggedInUser->get_my_content(ind);
                        break;
                    }
                    case 'p': {
                        ind--;
                        if (ind < 0){
                            ind++;
                            last_message = "no content prior to this";
                        }
                        loggedInUser->get_my_content(ind);
                        break;
                    }
                    case 'd': {
                        loggedInUser->remove_content(*(loggedInUser->contents[ind]));
                        if (loggedInUser->contents.size() == 0) {
                            menuState = MenuState::LOGGED_IN;
                            last_message = "content successfully deleted\nyou have no contents";
                        } else {
                            last_message = "content successfully deleted\n";
                        }
                        ind = 0;
                        break;
                    }
                    case 'e':{
                        cout<<"Please enter your text to replace:"<<endl;
                        cin>>user_text;
                        loggedInUser->contents[ind]->edit_content(user_text);
                        loggedInUser->get_my_content(ind);
                        break;
                    }
                    case 'b':{
                        ind = 0;
                        menuState = MenuState::LOGGED_IN;
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
