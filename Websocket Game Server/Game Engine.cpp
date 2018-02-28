#include "Game Engine.hpp"

GameEngine::GameEngine()
{
    
    startTime = localTime = timeLastFrame = std::chrono::duration_cast< std::chrono::milliseconds >(
               std::chrono::system_clock::now().time_since_epoch()).count();
    
}

/*
 EXTERNAL RUN CONTROL
 Controls for the running/pausing/execution of the game engine
 */

void GameEngine::Run(){
    startTime = localTime = timeLastFrame = std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()).count();
    isRunning = true;
    
    while(isRunning){
        if(deltaTime > 0)   //if there was an update last frame, then update the time of last frame
            timeLastFrame = localTime;
        
        //Sleep(1);
        usleep(100);
        localTime = std::chrono::duration_cast< std::chrono::milliseconds >(
                    std::chrono::system_clock::now().time_since_epoch()).count();
        deltaTime = localTime - timeLastFrame;
        //std::cout << (localTime - startTime) * MILLISECONDS_TO_SECONDS << " : " << deltaTime * MILLISECONDS_TO_SECONDS << std::endl;
        
        if(deltaTime > 0)   //will need to also prevent 'timeLastFrame' from being updated when not in this case
            GameTick(deltaTime * MILLISECONDS_TO_SECONDS);
    }
}

void GameEngine::ManualTick(){
    localTime = std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()).count();
    //std::cout << localTime << std::endl;
    deltaTime = localTime - timeLastFrame;
    if(deltaTime > 0)   //will need to also prevent 'timeLastFrame' from being updated when not in this case
        GameTick(deltaTime * MILLISECONDS_TO_SECONDS);
    
    timeLastFrame = std::chrono::duration_cast< std::chrono::milliseconds >(
                    std::chrono::system_clock::now().time_since_epoch()).count();
}

void GameEngine::Suspend(){
    localTime = std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()).count();
    
    isRunning = false;
    
    timeLastFrame = std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()).count();
}

void GameEngine::ManualTick(double manualDeltaTime){
    GameTick(deltaTime * MILLISECONDS_TO_SECONDS);
    timeLastFrame += manualDeltaTime;
}

/*
 CONTENT CONTROL
 Adding, removing and modifying content
 */

void GameEngine::AddCollider(Bounds* newBound){
    ballColliders.push_back(newBound);
}

void GameEngine::RemoveCollider(Bounds* oldBound){
    for(std::vector<Bounds*>::iterator i = ballColliders.begin(); i != ballColliders.end(); ++i){
        if(*i == oldBound){
            ballColliders.erase(i);
            return;
        }
    }
}


/*
 CORE GAME ENGINE
 update cycle and collisions
 */

void GameEngine::GameTick(double deltaTime){
    RunCollisions();
    Update(deltaTime);
    Cleanup();
}

void GameEngine::RunCollisions(){
    for(int i = 0; i < ballColliders.size(); ++i){
        for(int j = i+1; j < ballColliders.size(); ++j){
            Bounds::side hitSideA = ballColliders[i]->intersectionSide(ballColliders[j]);
            Bounds::side hitSideB = ballColliders[j]->intersectionSide(ballColliders[i]);
            if(hitSideA != Bounds::none){
                ballColliders[i]->onCollision(ballColliders[j], hitSideA);
            }
            if(hitSideB != Bounds::none){
                ballColliders[j]->onCollision(ballColliders[i], hitSideB);
            }
        }
    }
}

void GameEngine::Update(double deltaTime){
    for(int i = 0; i < ballColliders.size(); ++i){
        ballColliders[i]->onTick(deltaTime);
    }
}

void GameEngine::Cleanup(){
    for(int i = 0; i < ballColliders.size(); ++i){
        ballColliders[i]->onEndFrame();
    }
}
