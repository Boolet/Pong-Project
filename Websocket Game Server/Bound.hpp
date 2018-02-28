#ifndef Bound_hpp
#define Bound_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <map>
#include "Score.hpp"

#define DEFAULT_BOUND_NAME "Default"

class Bounds{
public:
    Bounds(double x = 0, double y = 0, double w = 0, double h = 0, std::string newName = DEFAULT_BOUND_NAME){
        //top left corner coordinates
        xPos = x;
        yPos = y;
        
        //bottom right offset
        width = w;
        height = h;
        
        name = newName;
        
        ID = currentID;
        ++currentID;
    }
    
    enum axis{
        xAxis, yAxis
    };
    
    enum side{
        top, bottom, right, left, none
    };
    
    class CollisionBehavior{
    public:
        virtual void OnCollision(Bounds* other){}
    };
    
    class ResetScore : public CollisionBehavior{
    public:
        ResetScore(Score* target):targetScore(target){};
        
        void OnCollision(Bounds* other) override;
        
        Score* targetScore;
    };
    
    class ScoreKeeper : public CollisionBehavior{
    public:
        ScoreKeeper(std::map<Bounds*, Score*>* newScoreMap):scoreMap(newScoreMap){}
        
        void AddWall(Bounds* wall){wallColliders.push_back(wall);}
        void OnCollision(Bounds* other) override;
        
        std::vector<Bounds*> wallColliders;
        std::map<Bounds*, Score*>* scoreMap;
        Bounds* lastHit = nullptr;
    };
    
    //getters
    std::string getName(){return name;}
    double getX(){return xPos;}
    double getY(){return yPos;}
    double getWidth(){return width;}
    double getHeight(){return height;}
    Bounds getRotatedClockwise();
    Bounds getRotatedClockwiseAroundPoint(double x, double y);
    int getID(){return ID;}
    
    //setters
    void setName(std::string newName){name = newName;}
    void setX(float newX){xPos = newX;}
    void setY(float newY){yPos = newY;}
    void setWidth(float newWidth){width = newWidth;}
    void setHeight(float newHeight){height = newHeight;}
    
    //mutators
    void addCollisionBehavior(CollisionBehavior* behave){onCollisionBehaviors.push_back(behave);}
    void addNonRepeatCollisionBehavior(CollisionBehavior* behave){onNonRepeatCollisionBehaviors.push_back(behave);}
    
    //game engine stuff
    bool isIntersecting(Bounds* other);
    side intersectionSide(Bounds* other);
    virtual void onCollision(Bounds* other, side onSide);
    virtual void onTick(double deltaTime){};
    void onEndFrame();
    
protected:
    double xPos, yPos, width, height;
    int ID;
    std::string name = DEFAULT_BOUND_NAME;
    std::vector<Bounds*> intersectLastFrame;
    std::vector<Bounds*> intersectors;
    
    Bounds intersectionArea(Bounds* other);
    std::vector<CollisionBehavior*> onCollisionBehaviors;
    std::vector<CollisionBehavior*> onNonRepeatCollisionBehaviors;
    
private:
    static int currentID;
    
};

#endif /* Bound_hpp */
