#include "yourserver.h"
#include "connectionclosedexception.h"
#include "inmemdatabase.h"
#include "database.h"
#include <iostream>
using namespace std;

//int YourServer::run(){
//    while(true){
//        auto conn = server.waitForActivity();
//        if(conn != nullptr){
//            try { 
//                cout << "Recieved" << endl;
//            } catch (ConnectionClosedException&){
//                server.deregisterConnection(conn);
//                cout << "Connection closed" << endl;
//            }
//        }
//    }
//}

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

    //Database db;
    if (stoi(argv[2]) == 1) {
        //db = InMemDatabase();
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
    
    while(true){
        auto conn = server.waitForActivity();
        if(conn != nullptr){
            try { 
                cout << "Recieved" << endl;
            } catch (ConnectionClosedException&){
                server.deregisterConnection(conn);
                cout << "Connection closed" << endl;
            }
        }
    }
    return 0;
}