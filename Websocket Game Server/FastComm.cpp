#include "Communication Protocol.hpp"

std::string MessageHandler::objectAddedMessage(Bounds* object){
    std::ostringstream os;
    os << OBJECT_ADDED << ":" << object->getID() << ":" << object->getX() << ":" << object->getY() << ":" << object->getWidth() << ":" << object->getHeight();
    return os.str();
}

std::string MessageHandler::objectUpdateMessage(Bounds* object){
    std::ostringstream os;
    os << OBJECT_MODIFIED << ":" << object->getID() << ":" << object->getX() << ":" << object->getY() << ":" << object->getWidth() << ":" << object->getHeight();
    return os.str();
}

std::string MessageHandler::scoreUpdateMessage(Score* score){
    std::ostringstream os;
    os << SCORE_CHANGED << ":" << score->getID() << ":" << score->ownerName << ":" << score->score;
    return os.str();
}

std::string MessageHandler::objectRemovedMessage(Bounds* object){
    std::ostringstream os;
    os << OBJECT_REMOVED << ":" << object->getID();
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

double MessageHandler::movementAmount(std::string message){
    return std::stod(message.substr(PLAYER_PREFIX_SIZE));
}

std::string MessageHandler::playerName(std::string message){
    return message.substr(PLAYER_PREFIX_SIZE);
}
