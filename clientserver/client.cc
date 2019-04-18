#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

void listNewsgroups(MessageHandler& m);
void createNewsgroup(MessageHandler& m);
void deleteNewsgroup(MessageHandler& m);
void listArticles(MessageHandler& m);
void createArticle(MessageHandler& m);
void deleteArticle(MessageHandler& m);
void getArticle(MessageHandler& m);
void end();
int inputToNbr(const string&);
string getLine();

/* Creates a client for the given args, if possible.
* Otherwise exits with error code.
*/
Connection init(int argc, char* argv[])
{
    if (argc != 3) {
        cerr << "Usage: myclient host-name port-number" << endl;
        exit(1);
    }
    int port = -1;
    try {
        port = stoi(argv[2]);
    } catch (exception& e) {
        cerr << "Wrong port number. " << e.what() << endl;
        exit(2);
    }
    Connection conn(argv[1], port);
    if (!conn.isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(3);
    }
    return conn;
}

int app(Connection& conn)
{
    string input;
    cout << "Enter command: " << endl;
    while(cin >> input){ 
        int choice = inputToNbr(input);
        MessageHandler m(conn);
        try {
            switch(choice){
                case 0: listNewsgroups(m);  break;
                case 1: createNewsgroup(m); break;
                case 2: deleteNewsgroup(m); break;
                case 3: listArticles(m);    break;
                case 4: createArticle(m);   break;
                case 5: deleteArticle(m);   break;
                case 6: getArticle(m);      break;
                case 7: end();              break;
                default: cout << "Wrong input, use \"LIST/CREATE/DELETE + NG\" or \"LIST/CREATE/DELETE/GET + ART\" WITHOUT SPACES or \"END\" to quit\n" << endl;
            }
            cout << "Enter command: " << endl;
        } catch (ConnectionClosedException&) {
            cout << " no reply from server. Exiting." << endl;
            return 1;
        }
    }
    cout << "\nexiting.\n"; 
    return 0;
}

int main(int argc, char* argv[])
{
    Connection conn = init(argc, argv);
    return app(conn);
}

int inputToNbr(const string& s){
    if(s.compare("LISTNG") == 0){
        return 0;
    } else if(s.compare("CREATENG") == 0){
        return 1;
    } else if(s.compare("DELETENG") == 0){
        return 2;
    } else if(s.compare("LISTART") == 0){
        return 3;
    } else if(s.compare("CREATEART") == 0){
        return 4;
    } else if(s.compare("DELETEART") == 0){
        return 5;
    } else if(s.compare("GETART") == 0){
        return 6;
    } else if(s.compare("END") == 0){
        return 7;
    }
    return 8;
}

string getLine(){
    string s;
    getline(cin, s);
    if(!s.size()) getline(cin, s);
    return s;
}

void listNewsgroups(MessageHandler& m){
    m.sendCode(Protocol::COM_LIST_NG);
    m.sendCode(Protocol::COM_END);
    Protocol ans = m.recvCode();
    //ans == ANS_LIST_NG
    auto size = m.recvIntParameter();
    if(size < 1){
        cout << "No newsgroups existing" << endl;
    } else { 
        cout << "Newsgroups:" << endl;
        for(auto i = 0; i != size; ++i){
            auto nbr = m.recvIntParameter();
            auto name = m.recvStringParameter();
            cout << nbr << ", " << name << endl;
        }
    }
    Protocol end = m.recvCode();
    //end == ANS_END
}
void createNewsgroup(MessageHandler& m){
    cout << "Enter newsgroup title: " << endl;
    string title = getLine();

    cout << title << endl;
    m.sendCode(Protocol::COM_CREATE_NG);
    m.sendStringParameter(title);
    m.sendCode(Protocol::COM_END);
    Protocol ans = m.recvCode();
    //ans == ANS_CREATE_NG
    Protocol status = m.recvCode();
    if(status == Protocol::ANS_ACK){
        cout << "Newsgroup created!" << endl;
    } else {
        Protocol err = m.recvCode();
        cout << "Newsgroup already exists!" << endl;
    }
    Protocol end = m.recvCode();
    //end == ANS_END
}
void deleteNewsgroup(MessageHandler& m){
    cout << "Enter newsgroup id: " << endl;
    long id; 
    cin >> id;
    if(!cin){
        cout << "Not a number! Exiting..." << endl;
        exit(1);
    }
    m.sendCode(Protocol::COM_DELETE_NG);
    m.sendIntParameter(id);
    m.sendCode(Protocol::COM_END);

    Protocol ans = m.recvCode();
    //ans == ANS_DELETE_NG
    Protocol status = m.recvCode();
    if(status == Protocol::ANS_ACK){
        cout << "Newsgroup deleted!" << endl;
    } else {
        Protocol err = m.recvCode();
        cout << "Newsgroup doesn't exist!" << endl;
    }
    Protocol end = m.recvCode();
    //end == ANS_END
}
void listArticles(MessageHandler& m){
    cout << "Enter newsgroup id: " << endl;
    long id;
    cin >> id;
    if(!cin){
        cout << "Not a number! Exiting..." << endl;
        exit(1);
    }
    m.sendCode(Protocol::COM_LIST_ART);
    m.sendIntParameter(id);
    m.sendCode(Protocol::COM_END);

    Protocol ans = m.recvCode();
    //ans == ANS_LIST_ART
    Protocol status = m.recvCode();
    if(status == Protocol::ANS_ACK){
        auto size = m.recvIntParameter();
        cout << "Articles:" << endl;
        for(auto i = 0; i != size; ++i){
            auto nbr = m.recvIntParameter();
            auto name = m.recvStringParameter();
            cout << nbr << ", " << name << endl;
        }
    } else {
        Protocol err = m.recvCode();
        cout << "Newsgroup dosen't exist!" << endl;
    }
    Protocol end = m.recvCode();
    //end == ANS_END
}

void createArticle(MessageHandler& m){
    long id; 
    string title;
    string author;
    string content;

    cout << "Enter newsgroup id: ";
    cin >> id; 
    if(!cin){
        cout << "Not a number! Exiting..." << endl;
        exit(1);
    }
    cout << "\nEnter title: ";
    cin >> title;
    cout << "\nEnter author: ";
    cin >> author;
    cout << "\nEnter text: ";
    cin >> content;
    
    m.sendCode(Protocol::COM_CREATE_ART);
    m.sendIntParameter(id);
    m.sendStringParameter(title);
    m.sendStringParameter(author);
    m.sendStringParameter(content);
    m.sendCode(Protocol::COM_END);

    Protocol ans = m.recvCode();
    //ans == ANS_CREATE_ART
    Protocol status = m.recvCode();
    if(status == Protocol::ANS_ACK){
        cout << "Article created!" << endl;
    } else {
        Protocol err = m.recvCode();
        cout << "Newsgroup dosen't exist!" << endl;
    }
    Protocol end = m.recvCode();
    //end == ANS_END
}
void deleteArticle(MessageHandler& m){
    long ngId;
    long artId;

    cout << "Enter newsgroup id: ";
    cin >> ngId;
    if(!cin){
        cout << "Not a number! Exiting..." << endl;
        exit(1);
    }
    cout << "\nEnter article id: ";
    cin >> artId;
    if(!cin){
        cout << "Not a number! Exiting..." << endl;
        exit(1);
    }

    m.sendCode(Protocol::COM_DELETE_ART);
    m.sendIntParameter(ngId);
    m.sendIntParameter(artId);
    m.sendCode(Protocol::COM_END);

    Protocol ans = m.recvCode();
    //ans == ANS_DELETE_ART
    Protocol status = m.recvCode();
    if(status == Protocol::ANS_ACK){
        cout << "Article deleted!" << endl;
    } else {
        Protocol noExist = m.recvCode();
        if(noExist == Protocol::ERR_NG_DOES_NOT_EXIST){
            cout << "Newsgroup doesn't exist!" << endl;
        } else {
            cout << "Article doesn't exist!" << endl;
        }
    }
    Protocol end = m.recvCode();
    //end == ANS_END
}

void getArticle(MessageHandler& m){
    long ngId;
    long artId;

    cout << "Enter newsgroup id: ";
    cin >> ngId;
    if(!cin){
        cout << "Not a number! Exiting..." << endl;
        exit(1);
    }

    cout << "\nEnter article id: ";
    cin >> artId;
    if(!cin){
        cout << "Not a number! Exiting..." << endl;
        exit(1);
    }

    m.sendCode(Protocol::COM_GET_ART);
    m.sendIntParameter(ngId);
    m.sendIntParameter(artId);
    m.sendCode(Protocol::COM_END);

    Protocol ans = m.recvCode();
    //ans == ANS_GET_ART
    Protocol status = m.recvCode();
    if(status == Protocol::ANS_ACK){
        string title = m.recvStringParameter();
        string author = m.recvStringParameter();
        string content = m.recvStringParameter();
        cout << title << ", " << author << ", " << content << endl;
    } else {
        Protocol noExist = m.recvCode();
        if(noExist == Protocol::ERR_NG_DOES_NOT_EXIST){
            cout << "Newsgroup doesn't exist!" << endl;
        } else {
            cout << "Article doesn't exist!" << endl;
        }
    }
    Protocol end = m.recvCode();
    //end == ANS_END
}

void end(){
    cout << "BYE" << endl;
    exit(1);
}