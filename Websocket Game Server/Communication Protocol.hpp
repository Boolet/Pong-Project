#ifndef Communication_Protocol_hpp
#define Communication_Protocol_hpp

#include <stdio.h>
#include <sstream>
#include <queue>
#include "Bound.hpp"
#include "Score.hpp"

#define SERVER_PREFIX_SIZE 6
#define OBJECT_ADDED "OBJADD"
#define OBJECT_MODIFIED "OBJMOD"
#define OBJECT_REMOVED "OBJDEL"
#define SCORE_CHANGED "SCRCHN"

#define PLAYER_PREFIX_SIZE 4
#define PLAYER_MOVE "MOVE"
#define PLAYER_HELLO "HELO"

class MessageHandler{
public:
    
    //sent message formats
    static std::string objectAddedMessage(Bounds* object);
    static std::string objectUpdateMessage(Bounds* object);
    static std::string scoreUpdateMessage(Score* score);
    static std::string objectRemovedMessage(Bounds* object);
    
    static std::string timestamp();
    static std::string timestamp(double time);
    static std::string appendTimestamp(std::string message, double time);
    
    //recieve message methods
    static double movementAmount(std::string message);
    static std::string playerName(std::string message);
    
    
    struct QueueMessage{
        QueueMessage(int newClientID, std::string newMessage, double newTimeQueued)
        :clientID(newClientID), message(newMessage), timeQueued(newTimeQueued){}
        
        int clientID;
        std::string message;
        double timeQueued;
    };
    
    
    //communication functions
    void queueOutgoingMessage(int clientID, std::string message, double timeQueued);
    void queueIncomingMessage(int clientID, std::string message, double timeQueued);
    
private:
    std::priority_queue<QueueMessage> outgoingQueue;
    std::priority_queue<QueueMessage> incomingQueue;
    
};

#endif /* Communication_Protocol_hpp */
