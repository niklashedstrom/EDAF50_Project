#include "connectionclosedexception.h"
#include "inmemdatabase.h"
#include "database.h"
#include "messagehandler.h"
#include "server.h"
#include <iostream>
#include <string>

using namespace std;

void listNewsGroups(Database& db, MessageHandler& m);
void createNewsGroup(Database& db, MessageHandler& m);
void deleteNewsGroup(Database& db, MessageHandler& m);
void listArticles(Database& db, MessageHandler& m);
void createArticle(Database& db, MessageHandler& m);
void deleteArticle(Database& db, MessageHandler& m);
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

    if (!((stoi(argv[2]) == 1) || (stoi(argv[2]) == 2))) {
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

    Database *db;
    if(stoi(argv[2]) == 1){
        db = new InMemDatabase();
        cout << "Created In memory DB!" << endl;
    } else {
        //db = new DiskDatabase();
        cout << "Created Disk DB!" << endl;
    }

    while(true){
        auto conn = server.waitForActivity();
        
        if(conn != nullptr){
            MessageHandler m{*conn.get()};
            try { 
                auto reviecedCode = m.recvCode(); 
                switch(reviecedCode){
                    case Protocol::COM_LIST_NG   : listNewsGroups(*db, m);   break; // list newsgroups
                    case Protocol::COM_CREATE_NG : createNewsGroup(*db , m); break; // create newsgroup
                    case Protocol::COM_DELETE_NG : deleteNewsGroup(*db, m);  break; // delete newsgroup
                    case Protocol::COM_LIST_ART  : listArticles(*db, m);     break; // list articles
                    case Protocol::COM_CREATE_ART: createArticle(*db, m);    break; // create article
                    case Protocol::COM_DELETE_ART: deleteArticle(*db, m);    break; // delete article
                    case Protocol::COM_GET_ART   : getArticle(*db, m);       break; // get article
                }
                m.sendCode(Protocol::ANS_END);
                if(Protocol::COM_END != m.recvCode()){
                    cout << "End.." << endl;
                }
            } catch (ConnectionClosedException&){
                server.deregisterConnection(conn);
                cout << "Connection closed" << endl;
            }
        } else {
            conn = make_shared<Connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
        }
    }
    delete db;
    return 0;
}

void listNewsGroups(Database& db, MessageHandler& m){
    m.sendCode(Protocol::ANS_LIST_NG);
    auto tmpList = db.getNewsGroups();
    m.sendIntParameter(tmpList.size());
    for(auto& ng : tmpList){
        m.sendIntParameter(ng.id);
        m.sendStringParameter(ng.name);
    }
}

void createNewsGroup(Database& db, MessageHandler& m){
    m.sendCode(Protocol::ANS_CREATE_NG);
    auto name = m.recvStringParameter();
    auto tmpList = db.getNewsGroups();
    bool flag = true;
    if(!tmpList.empty()){
        auto it = tmpList.begin();
        while(it != tmpList.end()){
            auto ng = *it;
            cout << ng.name << endl; 
            if(ng.name == name){
                flag = false;
            }
            ++it;    
        }

    } else { 
        flag = true;
    }
    if(flag){
        db.addNewsGroup(name);
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

    bool flag = false;
    for(auto& ng : db.getNewsGroups()){
        if (ng.id == ngId){
            flag = true;
        }
    }
    if(flag){
        m.sendCode(Protocol::ANS_ACK);
        m.sendIntParameter(art.size());
        for(auto& a : art){
            m.sendIntParameter(a.id);
            m.sendStringParameter(a.title);
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

    bool flag = false;
    for(auto& ng : db.getNewsGroups()){
        if (ng.id == ngId){
            flag = true;
        }
    }

    if(flag){
        Article a;
        a.title = title;
        a.author = author;
        a.content = text;
        db.addArticle(ngId, a);
        m.sendCode(Protocol::ANS_ACK);
    } else { 
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
}

void deleteArticle(Database& db, MessageHandler& m){
    m.sendCode(Protocol::ANS_DELETE_ART);
    auto ngId = m.recvIntParameter();
    auto artId = m.recvIntParameter();

    auto art = db.getArticles(ngId);
    bool flag = false;
    bool anotherflag = false;

    if(!art.empty()){
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
        m.sendCode(Protocol::ANS_ACK);
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

    bool ngflag = false;
    for(auto& ng : db.getNewsGroups()){
        if(ng.id == ngId) ngflag = true;
    }
    if(!ngflag){
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
        return;
    }

    bool artflag = false;
    Article temp;
    for(auto& a : db.getArticles(ngId)){
        if(a.id == artId){ 
            artflag = true;
            temp = a;
        }
    }
    if(artflag){
        m.sendCode(Protocol::ANS_ACK);
        m.sendStringParameter(temp.title);
        m.sendStringParameter(temp.author);
        m.sendStringParameter(temp.content);
    } else { 
        m.sendCode(Protocol::ANS_NAK);
        m.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
    }
}