#ifndef Paddle_hpp
#define Paddle_hpp

#include <stdio.h>
#include "Bound.hpp"
#include "Score.hpp"

class Paddle : public Bounds{
    
public:
    Paddle(float x = 0, float y = 0, float w = 0, float h = 0):Bounds(x,y,w,h){/*paddleScore = new Score();*/}
    
    /*
    void onCollision(Bounds* other, side onSide) override;
    
    int getScoreValue(){return paddleScore->score;}
    Score* getScoreRef(){return paddleScore;}
private:
    Score* paddleScore;
    */
};

#endif /* Paddle_hpp */
