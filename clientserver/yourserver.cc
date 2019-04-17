#include "yourserver.h"
#include "connectionclosedexception.h"
#include "inmemdatabase.h"
#include "database.h"
#include "messagehandler.h"
#include <iostream>
using namespace std;

// list newsgroups
void listNewsGroups(Database& db, MessageHandler& m);
// create newsgroup
void createNewsGroup(Database& db, MessageHandler& m);
// delete newsgroup
void deleteNewsGroup(Database& db, MessageHandler& m);
// list articles
void listArticles(Database& db, MessageHandler& m);
// create article
void createArticle(Database& db, MessageHandler& m);
// delete article
void deleteArticle(Database& db, MessageHandler& m);
// get article
void getArticle(Database& db, MessageHandler& m);


Server init(int argc, char* argv[]){
    if (argc != 3) {
        std::cout << "Wrong number of arguments, first portnumber then 1 for in mem db, 2 for disk db" << std::endl;
        exit(1);
    }
    int port = -1;
    try { 
        port = stoi(argv[1]);
    } catch (exception& e){
        cerr << "Wrong format for portnumber." << e.what() << endl;
        exit(2);
    }

    Database db;
    if (stoi(argv[2]) == 1) {
        db = InMemDatabase();
    } else if (stoi(argv[2]) == 2) {
        // Create new disk database
    } else {
        std::cout << "Wrong argument, 1 for in mem db, 2 for disk db" << std::endl;
        exit(3);
    }

    Server server(port);
    if(!server.isReady()){
        cerr << "Server init error" << endl;
        exit(4);
    }
    return server;
}

int main(int argc, char* argv[]) {
    
    auto server = init(argc, argv);

    Database db;
    if(stoi(argv[2]) == 1){
        db = InMemDatabase();
    } else {
        //disk
    }
    
    while(true){
        auto conn = server.waitForActivity();
        MessageHandler m(conn);
        if(conn != nullptr){
            cout << "Reached" << endl;
            try { 
                cout << "Recieved" << endl;
                switch(m.recvCode()){
                    case Protocol::COM_LIST_NG   : listNewsGroups(db, m); break; // list newsgroups
                    case Protocol::COM_CREATE_NG : // create newsgroup
                    case Protocol::COM_DELETE_NG : // delete newsgroup
                    case Protocol::COM_LIST_ART  : // list articles
                    case Protocol::COM_CREATE_ART: // create article
                    case Protocol::COM_DELETE_ART: // delete article
                    case Protocol::COM_GET_ART   : // get article
                    case Protocol::COM_END       : // command end
                }
                m.sendCode(Protocol::ANS_END);
            } catch (ConnectionClosedException&){
                server.deregisterConnection(conn);
                cout << "Connection closed" << endl;
            }
        } else {
            conn = make_shared<Connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
        }
        cout << "hejdu" << endl;
    }
    return 0;
}


void listNewsGroups(Database& db, MessageHandler& m){
    m.sendCode(Protocol::ANS_LIST_NG);
    auto tmpList = db.getNewsGroups();
    m.sendIntParameter(tmpList.size());
    for(auto& ng : tmpList){
        m.sendIntParameter(ng.id);
        m.sendStringParamter(ng.name);
    }
}

void createNewsGroup(Database& db, MessageHandler& m){
    m.sendCode(Protocol::ANS_CREATE_NG);
    auto name = m.recvStringParameter();
    auto tmpList = db.getNewsGroups();
    bool flag = true;
    for(auto& ng : tmpList){
        if(ng.name == name){
            flag = false;
        }
    }
    if(flag){
        db.createNewsGroup(name);
        m.sendCode(Protocol::ANS_ACK);
    } else {
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
    }
}

void deleteNewsGroup(Database& db, MessageHandler& m){
    m.sendCode(Protocol::ANS_DELETE_NG);
    auto id = m.recvIntParameter();
    auto tmpList = db.getNewsGroups();
    bool flag = false;
    for(auto& ng : tmpList){
        if(ng.id == id){
            flag = true;
        }
    }
    if(flag){
        db.removeNewsGroup(id);
        m.sendCode(Protocol::ANS_ACK);
    } else {
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
}

void listArticles(Database& db, MessageHandler& m){
    m.sendCode(Protocol::ANS_LIST_ART);
    auto ngId = m.recvIntParameter();
    auto art = db.getArticles(ngId);
    if(art != NULL){
        m.sendCode(Protocol::ANS_ACK);
        m.sendIntParameter(art.size());
        for(auto& a : art){
            m.sendIntParameter(a.id);
            m.sendStringParamter(a.title);
        }
    } else { 
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
}

void createArticle(Database& db, MessageHandler& m){
    m.sendCode(Protocol::ANS_CREATE_ART);
    auto ngId = m.recvIntParameter();
    auto title = m.recvStringParameter();
    auto author = m.recvStringParameter();
    auto text = m.recvStringParameter();
    
    auto art = db.getArticles(ngId);
    if(art != NULL){
        Article a;
        a.title = title;
        a.author = author;
        a.content = text;
        db.createArticle(a);
        m.sendCode(Protocol::ANS_ACK);
    } else { 
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
}

void deleteArticle(Database& db, MessageHandler& m){
    m.sendCode(ANS_DELETE_ART);
    auto ngId = m.recvIntParameter();
    auto artId = m.recvIntParameter();

    auto art = db.getArticles(ngId);
    bool flag = false;
    bool anotherflag = false;

    if(art != NULL){
        for(auto& a : art){
            if(a.id == artId){
                flag = true;
            }
        }
    } else { 
        anotherflag = true;
    }
    if(flag){
        db.removeArticle(ngId, artId);
        m.sendCode(ANS_ACK);
    } else {
        m.sendCode(Protocol::ANS_NAK);
        if(anotherflag) { 
            m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
        } else {
            m.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
        }
    }
}

void getArticle(Database& db, MessageHandler& m){
    m.sendCode(Protocol::ANS_GET_ART);
    auto ngId = m.recvIntParameter();
    auto artId = m.recvIntParameter();

    auto art = db.getArticles(ngId);
    bool flag = false;
    bool anotherflag = false;

    if(art != NULL){
        for(auto& a : art){
            if(a.id == artId){
                flag = true;
            }
        }
    } else { 
        anotherflag = true;
    }
    if(flag){
        m.sendStringParameter(art[artId]); //Hämtar artikeln
        m.sendCode(ANS_ACK);
    } else {
        m.sendCode(Protocol::ANS_NAK);
        if(anotherflag) { 
            m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
        } else {
            m.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
        }
    }
}