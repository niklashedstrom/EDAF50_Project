#include "messagehandler.h"

using std::string;

MessageHandler::MessageHandler(Connection& c): conn(&c){}

void MessageHandler::sendCode(Protocol code){
    sendByte(static_cast<unsigned char>(code));
}

void MessageHandler::sendInt(int value){
    conn->write((value >> 24) & 0xFF);
    conn->write((value >> 16) & 0xFF);
    conn->write((value >> 8) & 0xFF);
    conn->write(value & 0xFF);
}

void MessageHandler::sendIntParameter(int value){
    sendCode(Protocol::PAR_NUM);
    sendInt(value);
}

void MessageHandler::sendStringParameter(string& value){
    sendCode(Protocol::PAR_STRING);
    sendInt(value.size());
    for(char c : value){
        sendByte(c);
    }
}

Protocol MessageHandler::recvCode(){
    unsigned char c = recvByte();
    return static_cast<Protocol>(c);
}

int MessageHandler::recvInt(){
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

int MessageHandler::recvIntParamter(){
    auto c = recvByte();
    if(c != Protocol::PAR_NUM){
        throw ConnectionClosedException();
    }
    return recvInt();
}

string MessageHandler::recvStringParameter(){
    auto c = recvByte();
    if(c != Protocol::PAR_STRING){
        throw ConnectionClosedException();
    }
    int p = recvInt();
    if(p < 0){
        throw ConnectionClosedException();
    }
    string s = resize(p);
    for(int i = 0; i < p.size(); i++){
        s[i] =  conn->read();
    }
    return s;

}

void MessageHandler::sendByte(unsigned char value){
    conn->write(value);
}

unsigned char MessageHandler::recvByte(){
    return conn->read();
}