#ifndef Communication_Protocol_hpp
#define Communication_Protocol_hpp

#include <stdio.h>
#include <sstream>
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
    
    //new - send methods
    static std::string objectAddedMessage(Bounds* object);
    static std::string objectUpdateMessage(Bounds* object);
    static std::string scoreUpdateMessage(Score* score);
    static std::string objectRemovedMessage(Bounds* object);
    
    static std::string timestamp(double time);
    static std::string appendTimestamp(std::string message, double time);
    
    //new - recieve methods
    static double movementAmount(std::string message);
    static std::string playerName(std::string message);
};

#endif /* Communication_Protocol_hpp */
