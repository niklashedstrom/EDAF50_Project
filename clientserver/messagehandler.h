#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include <string>


class MessageHandler {
public:
    MessageHandler(Connection& conn);
    void sendCode(Protocol code);
    void sendInt(int value);
    void sendIntParameter(int value);
    void sendStringParameter(std::string& value);
    Protocol recvCode();
    int recvInt();
    int recvIntParameter();
    std::string recvStringParameter();
private:
    Connection* conn;
    void sendByte(unsigned char value);
    unsigned char recvByte();
};


#endif