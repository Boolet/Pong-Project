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

#define NO_LATENCY 0
#define FIXED_LATENCY 1
#define RANDOM_LATENCY 2
#define INCREMENTAL_LATENCY 3

class MessageHandler{
public:
    
    //static methods
    
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
        QueueMessage():clientID(-1), message(""), timeQueued(-1){}
        QueueMessage(int newClientID, std::string newMessage, double newTimeQueued)
        :clientID(newClientID), message(newMessage), timeQueued(newTimeQueued){}
        
        int clientID;
        std::string message;
        double timeQueued;
    };
    
    //instance methods
    MessageHandler(double minLatency, double maxLatency, double growthPerSecond, int setMode = NO_LATENCY)
        :minimumLatency(minLatency), maximumLatency(maxLatency), latencyGrowthPerSecond(growthPerSecond), mode(setMode){}
    
    void setStartTime(double time){startTime = time;}
    
    //logging functions
    /*
     For each message that you want to send or recieve that needs latency added,
     call the appropriate method and pass in the client, the message, and the time
     that it was recieved or intended to be sent.
     */
    void queueOutgoingMessage(int clientID, std::string message, double timeQueued);
    void queueIncomingMessage(int clientID, std::string message, double timeQueued);
    
    //reading functions
    /*
     When you want to handle an incoming or outgoing message, call the appropriate
     method here. Since multiple messages may be ready to be handled at any given time,
     it is best to put the method inside a while loop.
     
     When there are no messages left to be handled, the returned QueueMessage will have
     clientID of -1.
     */
    QueueMessage popOutgoingMessage(double currentTime);
    QueueMessage popIncomingMessage(double currentTime);
    
private:
    int mode = NO_LATENCY;
    double minimumLatency = 0.0;
    double maximumLatency = 0.0;
    double latencyGrowthPerSecond = 0.0;
    double startTime = 0.0;
    
    double getLatency(double timeQueued);
    double fixedLatency();
    double randomLatency();
    double incrimentalLatency(double timeQueued);
    
    bool queueMessageComparator(QueueMessage a, QueueMessage b){return a.timeQueued > b.timeQueued;}
    std::priority_queue<QueueMessage, std::vector<QueueMessage>, std::function<bool(QueueMessage,QueueMessage)>> outgoingQueue;
    std::priority_queue<QueueMessage, std::vector<QueueMessage>, std::function<bool(QueueMessage,QueueMessage)>> incomingQueue;
    
};

#endif /* Communication_Protocol_hpp */
