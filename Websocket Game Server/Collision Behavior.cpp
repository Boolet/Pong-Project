//
//  Collision Behavior.cpp
//  Websocket Game Server
//
//  Created by Avi Miller on 2/15/18.
//  Copyright © 2018 Avi Miller. All rights reserved.
//

#include "Bound.hpp"

void Bounds::ResetScore::OnCollision(Bounds* other){
    if(other->getName() == "ball"){
        targetScore->score = 0;
    }
}

void Bounds::ScoreKeeper::OnCollision(Bounds* other){
    if(scoreMap->find(other) != scoreMap->end()){
        lastHit = other;
        //std::cout << "Found paddle: " << other->getID() << std::endl;
        return;
    }
    
    if(lastHit == nullptr)
        return;
    
    for(auto a : wallColliders){
        if(a == other){
            ++scoreMap->at(lastHit)->score;
            //std::cout << "Found wall: " << other->getID() << std::endl;
            return;
        }
    }
}
