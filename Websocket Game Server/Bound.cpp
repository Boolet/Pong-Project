#include "Bound.hpp"

int Bounds::currentID = 0;

bool Bounds::isIntersecting(Bounds* other){
    return !(other->xPos + other->width < xPos ||
            other->yPos + other->height < yPos ||
            xPos + width < other->xPos ||
            yPos + height < other->yPos);
}

Bounds Bounds::intersectionArea(Bounds* other){
    double left = std::max(xPos, other->xPos);
    double top = std::max(yPos, other->yPos);
    
    return Bounds(left,
                  top,
                  std::min(xPos+width,other->xPos+other->width) - left,
                  std::min(yPos+height,other->yPos+other->height) - top);
}

Bounds::side Bounds::intersectionSide(Bounds* other){
    if(!isIntersecting(other))
        return none;
    
    Bounds collisionArea = intersectionArea(other);
    /*
    std::cout << "other xPos: " << other->xPos << ", yPos: " << other->yPos <<
    ", width: " << other->width << ", height: " << other->height << std::endl;
    std::cout << "collision overlap xPos: " << collisionArea.xPos << ", yPos: " << collisionArea.yPos <<
    ", width: " << collisionArea.width << ", height: " << collisionArea.height << std::endl;
     */
    
    if(collisionArea.width > collisionArea.height){
        //top or bottom
        if(collisionArea.yPos == yPos)
            return top;
        else return bottom;
    } else {
        //right or left
        if(collisionArea.xPos == xPos)
            return left;
        else return right;
    }
}

void Bounds::onCollision(Bounds* other, side onSide){
    //std::cout << "Collided!" << std::endl;
    intersectors.push_back(other);
    for(auto a : onCollisionBehaviors){
        a->OnCollision(other);
    }
    if(onNonRepeatCollisionBehaviors.size() > 0){
        for(auto a : intersectLastFrame){
            if(a == other)
                return;
        }
        for(auto a: onNonRepeatCollisionBehaviors){
            a->OnCollision(other);
        }
    }
}

void Bounds::onEndFrame(){
    intersectLastFrame = intersectors;
    intersectors.clear();
}

Bounds Bounds::getRotatedClockwise(){
    double rotHeight = width;
    double rotWidth = height;
    return Bounds(xPos, yPos, rotWidth, rotHeight);
}

//unfinished
Bounds Bounds::getRotatedClockwiseAroundPoint(double x, double y){
    double xOffset = (xPos - x/2) / (x/2);
    double yOffset = (yPos - y/2) / (y/2);
    if(xOffset < 0){
        if(yOffset < 0){
            
        }
    }
    
    return Bounds();
}

