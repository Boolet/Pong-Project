//
//  main.cpp
//  Learning Sockets
//
//  Created by Avi Miller on 2/15/18.
//  Copyright © 2018 Avi Miller. All rights reserved.
//

#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <map>
#include <sstream>
#include <chrono>
#include <map>
#include "websocket.hpp"
#include "Game Engine.hpp"
#include "Communication Protocol.hpp"

#define PORT 8888
#define TIME_BETWEEN_MESSAGES 10
#define PLAY_WIDTH 500
#define PLAY_HEIGHT 500
#define MAX_PLAYERS 4
#define MIN_PLAYERS 4

webSocket server;
GameEngine* engine = new GameEngine();
Paddle* paddles[4];
Bounds* walls[4];
Ball* ball;
std::map<Bounds*, Score*> scoreboard;

void initGameEngine(){
    //add the objects here
    
    //paddles for each player
    engine->AddCollider(paddles[0] = new Paddle(10, PLAY_HEIGHT/2, 5, PLAY_HEIGHT/6));
    engine->AddCollider(paddles[1] = new Paddle(PLAY_WIDTH/2, 10, PLAY_WIDTH/6, 5));
    engine->AddCollider(paddles[2] = new Paddle(PLAY_WIDTH-10, PLAY_HEIGHT/2, 5, PLAY_HEIGHT/6));
    engine->AddCollider(paddles[3] = new Paddle(PLAY_WIDTH/2, PLAY_HEIGHT-10, PLAY_WIDTH/6, 5));
    
    //walls
    engine->AddCollider(walls[0] = new Bounds(0, 0, 5, PLAY_HEIGHT));
    engine->AddCollider(walls[1] = new Bounds(0, 0, PLAY_WIDTH, 5));
    engine->AddCollider(walls[2] = new Bounds(PLAY_WIDTH, 0, 5, PLAY_HEIGHT));
    engine->AddCollider(walls[3] = new Bounds(0, PLAY_HEIGHT, PLAY_WIDTH, 5));
    
    //ball
    engine->AddCollider(ball = new Ball(PLAY_WIDTH/2, PLAY_HEIGHT/2));
    
    //create the scoreboard
    scoreboard[paddles[0]] = new Score();
    scoreboard[paddles[1]] = new Score();
    scoreboard[paddles[2]] = new Score();
    scoreboard[paddles[3]] = new Score();
    
    //add behaviors
    Bounds::ScoreKeeper* scoreBehavior = new Bounds::ScoreKeeper(&scoreboard);
    scoreBehavior->AddWall(walls[0]);
    scoreBehavior->AddWall(walls[1]);
    scoreBehavior->AddWall(walls[2]);
    scoreBehavior->AddWall(walls[3]);
    ball->addCollisionBehavior(scoreBehavior);
}

void stepGameEngine(double deltaTime){
    engine->ManualTick(deltaTime);
}

void sendGameData(){
    std::string data;
    
    std::vector<int> clientIDs = server.getClientIDs();
    for (int i = 0; i < clientIDs.size(); i++){
        for (int j = 0; j < 4; ++j) {
            server.wsSend(clientIDs[i], MessageHandler::objectUpdateMessage(paddles[j]));
            server.wsSend(clientIDs[i], MessageHandler::scoreUpdateMessage(scoreboard[paddles[j]]));
        }
        server.wsSend(clientIDs[i], MessageHandler::objectUpdateMessage(ball));
    }
}

/*
 The client's ID is going to be a direct index into the paddle array
 */
void updateGameData(int clientID, std::string message){
    double amount = MessageHandler::movementAmount(message);
    switch(clientID % 2){
        case 0:
            paddles[clientID]->setY(paddles[clientID]->getY()+amount);
            break;
        case 1:
            paddles[clientID]->setX(paddles[clientID]->getX()+amount);
            break;
    }
}

void setPlayerName(int clientID, std::string message){
    scoreboard[paddles[clientID]]->ownerName = MessageHandler::playerName(message);
}

//detemines what type of message it is and calls the correct handler
void switchOnMessageType(int clientID, std::string message){
    if(message.substr(0, PLAYER_PREFIX_SIZE) == PLAYER_HELLO){
        setPlayerName(clientID, message);
    } else if(message.substr(0, PLAYER_PREFIX_SIZE) == PLAYER_MOVE){
        updateGameData(clientID, message);
    } else {
        std::cout << "Incorrect message format:" << message << std::endl;
    }
}


/* called when a client connects */
void openHandler(int clientID){
    std::vector<int> clientIDs = server.getClientIDs();
    
    std::cout << "New client ID is: " << clientID << ". Current number of clients: " << clientIDs.size() << std::endl;
    
    if(clientIDs.size() > MAX_PLAYERS){
        server.wsClose(clientID);
        return;
    }
    
    //send information for every object
    for (int i = 0; i < 4; ++i) {
        server.wsSend(clientID, MessageHandler::objectAddedMessage(paddles[i]));
        server.wsSend(clientID, MessageHandler::objectAddedMessage(walls[i]));
        server.wsSend(clientID, MessageHandler::scoreUpdateMessage(scoreboard[paddles[i]]));
    }
    server.wsSend(clientID, MessageHandler::objectAddedMessage(ball));
}

/* called when a client disconnects */
void closeHandler(int clientID){
    std::cout << "Client " << clientID <<" disconnected" << std::endl;
    if(clientID < MAX_PLAYERS)
        scoreboard[paddles[clientID]]->ownerName = DEFAULT_SCORE_NAME;
}

/*
 Will need to map each player's input to a unique paddle and save their playerID
 */

/* called when a client sends a message to the server */
void messageHandler(int clientID, std::string message){
    std::cout << "Input from " << clientID << " containing: " << message << std::endl;
    switchOnMessageType(clientID, message);
}

/* called once per select() loop */
void periodicHandler(){
    static double next = std::chrono::duration_cast< std::chrono::milliseconds >(
                        std::chrono::system_clock::now().time_since_epoch()).count() + TIME_BETWEEN_MESSAGES;
    double current = std::chrono::duration_cast< std::chrono::milliseconds >(
                        std::chrono::system_clock::now().time_since_epoch()).count();
    
    std::vector<int> clientIDs = server.getClientIDs();
    if(clientIDs.size() >= MIN_PLAYERS){
        engine->ManualTick();
    }else{
        engine->Suspend();
    }
    
    if (current >= next){   //send a message every 1 seconds
        //std::cout << "Number of clients: " << clientIDs.size() << std::endl;
        sendGameData();
        next = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()).count() + TIME_BETWEEN_MESSAGES;
    }
}

int main(int argc, char *argv[]){
    
    initGameEngine();
    
    /* set event handler */
    server.setOpenHandler(openHandler);
    server.setCloseHandler(closeHandler);
    server.setMessageHandler(messageHandler);
    server.setPeriodicHandler(periodicHandler);
    
    /* start the chatroom server, listen to ip '127.0.0.1' and port '8888' */
    server.startServer(PORT);
    
    return 1;
}

