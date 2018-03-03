#include "Communication Protocol.hpp"

std::string MessageHandler::objectAddedMessage(Bounds* object){
    std::ostringstream os;
    os << OBJECT_ADDED << ":" << object->getID() << ":" << object->getX() << ":" << object->getY() << ":" << object->getWidth() << ":" << object->getHeight() << ":" << timestamp();
    return os.str();
}

std::string MessageHandler::objectUpdateMessage(Bounds* object){
    std::ostringstream os;
    os << OBJECT_MODIFIED << ":" << object->getID() << ":" << object->getX() << ":" << object->getY() << ":" << object->getWidth() << ":" << object->getHeight() << ":" << timestamp();
    return os.str();
}

std::string MessageHandler::scoreUpdateMessage(Score* score){
    std::ostringstream os;
    os << SCORE_CHANGED << ":" << score->getID() << ":" << score->ownerName << ":" << score->score << ":" << timestamp();
    return os.str();
}

std::string MessageHandler::objectRemovedMessage(Bounds* object){
    std::ostringstream os;
    os << OBJECT_REMOVED << ":" << object->getID() << ":" << timestamp();
    return os.str();
}

std::string MessageHandler::timestamp(double time){
    std::ostringstream os;
    os << time;
    return os.str();
}

std::string MessageHandler::appendTimestamp(std::string message, double time){
    std::ostringstream os;
    os << message << ":" << timestamp(time);
    return os.str();
}

std::string MessageHandler::timestamp(){
    double currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(
                                    std::chrono::system_clock::now().time_since_epoch()).count();
    return timestamp(currentTime);
}

double MessageHandler::movementAmount(std::string message){
    return std::stod(message.substr(PLAYER_PREFIX_SIZE));
}

std::string MessageHandler::playerName(std::string message){
    return message.substr(PLAYER_PREFIX_SIZE);
}


//Methods

double MessageHandler::fixedLatency(){
    return maximumLatency > minimumLatency ? maximumLatency : minimumLatency;
}

double MessageHandler::randomLatency(){
    return minimumLatency + (std::rand() / (double)RAND_MAX) * (maximumLatency - minimumLatency);
}

double MessageHandler::incrimentalLatency(double timeQueued){
    double latency = (timeQueued - startTime) * latencyGrowthPerSecond / 1000;
    latency = minimumLatency < latency ? latency : minimumLatency;
    latency = maximumLatency > latency ? latency : maximumLatency;
    return latency;
}

double MessageHandler::getLatency(double timeQueued){
    double latency = 0.0;
    switch (mode) {
        case FIXED_LATENCY:
            latency = fixedLatency();
            break;
        case RANDOM_LATENCY:
            latency = randomLatency();
            break;
        case INCREMENTAL_LATENCY:
            latency = incrimentalLatency(timeQueued);
            break;
        default:
            break;
    }
    return latency;
}

void MessageHandler::queueOutgoingMessage(int clientID, std::string message, double timeQueued){
    double latency = getLatency(timeQueued);
    outgoingQueue.push(QueueMessage(clientID, message, timeQueued + latency));
}

void MessageHandler::queueIncomingMessage(int clientID, std::string message, double timeQueued){
    double latency = getLatency(timeQueued);
    incomingQueue.push(QueueMessage(clientID, message, timeQueued + latency));
}

MessageHandler::QueueMessage MessageHandler::popIncomingMessage(double currentTime){
    if(incomingQueue.empty())
        return QueueMessage();
    
    QueueMessage top = incomingQueue.top();
    
    if(top.timeQueued < currentTime){
        return QueueMessage();
    }
    
    incomingQueue.pop();
    return top;
}

MessageHandler::QueueMessage MessageHandler::popOutgoingMessage(double currentTime){
    if(outgoingQueue.empty())
        return QueueMessage();
    
    QueueMessage top = outgoingQueue.top();
    
    if(top.timeQueued < currentTime){
        return QueueMessage();
    }
    
    outgoingQueue.pop();
    return top;
}



