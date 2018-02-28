#include "Ball.hpp"

void Ball::onTick(double deltaTime){
    xPos += xVelocity * deltaTime;
    yPos += yVelocity * deltaTime;
    //std::cout << xPos << " " << yPos << " " << deltaTime << std::endl;
}

void Ball::onCollision(Bounds* other, side onSide){
    Bounds::onCollision(other, onSide);
    switch (onSide) {
        case Bounds::top:
            //std::cout << "top of ball!" << std::endl;
            yVelocity = std::abs(yVelocity);
            break;
        case Bounds::right:
            //std::cout << "right of ball!" << std::endl;
            xVelocity = -std::abs(xVelocity);
            break;
        case Bounds::bottom:
            //std::cout << "bottom of ball!" << std::endl;
            yVelocity = -std::abs(yVelocity);
            break;
        case Bounds::left:
            //std::cout << "left of ball!" << std::endl;
            xVelocity = std::abs(xVelocity);
            break;
        default:
            break;
    }
}
